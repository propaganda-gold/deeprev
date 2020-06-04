const objects = require("../models/objects")
const cheerio = require('cheerio');
const getUrls = require('get-urls');
const fetch = require('node-fetch');
const mongoose = require("mongoose")

async function ScrapeMetatagsFromUrl(url) {
    console.log('ScrapeMetatagsFromUrl', 'url', url)
    const res = await fetch(url);
    const html = await res.text();
    const $ = cheerio.load(html);
    const getMetatag = (name) =>
        $(`meta[name=${name}]`).attr('content') ||
        $(`meta[name="og:${name}"]`).attr('content') ||
        $(`meta[name="twitter:${name}"]`).attr('content');
    return {
        url,
        title: $('title').first().text(),
        favicon: $('link[rel="shortcut icon"]').attr('href'),
        // description: $('meta[name=description]').attr('content'),
        description: getMetatag('description'),
        image: getMetatag('image'),
        author: getMetatag('author'),
    }
}

// BLOCKING function. Will BLOCK to cache a resource if this isn't cached yet.
//
// Steps:
//   1) Check if the resource is in the database yet.
//   2) If so, return from there.
//   3) If not:
//     4) scrape the page
//     5) update the database asynchronously
//     6) return the result asynchronously to the user
//
// NOTE: We just want to start with some version and iterate.
async function BlockingResouceFromUrl(url) {
    console.log('BlockingResouceFromUrl', 'url', url)

    var resolve_ = null
    var reject_ = null
    try {
        console.log('BlockingResouceFromUrl', 'trying')
        const promise = objects.Resource.findOne({
            url
        })
        const result = await promise
        console.log('BlockingResouceFromUrl', 'result', result)

        console.log('BlockingResouceFromUrl', 'result', result)
        if (result) {
            // Step 2: If so, return from here.
            resolve_ = result
        } else {
            // Step 3: Result not found.
            // Assume: result is null
            // Step 4: Scrape the page.
            const scrapedTuple = await ScrapeMetatagsFromUrl(url)
            console.log('BlockingResouceFromUrl', 'scrapedTuple', scrapedTuple)
            const bootstrap = {
                _id: new mongoose.Types.ObjectId(),
                url,
            }
            const merged = {
                ...bootstrap,
                ...scrapedTuple
            }
            console.log('BlockingResouceFromUrl', 'merged', merged)
            const resource = new objects.Resource(merged)
						
						resource.save().then(saveResult => {
								console.log('asynchronous save', 'saveResult', saveResult)
						}).catch(saveError => {
								console.log('asynchronous save', 'saveError', saveError)
						})

            console.log('BlockingResouceFromUrl', 'resource', resource)
            resolve_ = addedResource
        }
    } catch (error) {
        console.log('BlockingResouceFromUrl', 'error', error)
        reject_ = error
    }

    return new Promise(function(resolve, reject) {
        if (reject_) reject(reject_)
        if (resolve_) resolve(resolve_)
        reject({
            error: "either reject_ or resolve_ must be non-null"
        })
    })
}

module.exports = {
    BlockingResouceFromUrl,
}
