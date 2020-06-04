const objects = require("../models/objects")
const resources = require("../util/resources")
const createBroadcast = require("../util/createBroadcast")
const mongoose = require('mongoose')

const uri = process.env.DEEPREV_MONGOOSE_URI
console.log('mongoose uri', uri)
mongoose.connect(uri, {
    useNewUrlParser: true,
    useUnifiedTopology: true
})

const textarea = process.argv[2]
if (textarea == null) {
    console.log('textarea is null')
    process.exit(1)
}

const userId = process.argv[3]
if (userId == null) {
    console.log('userId is null')
    process.exit(1)
}

console.log('textarea', textarea)

//async function EmbellishResource(resource) {
//		await objects.Resource.findById(resourceId).then(resource => {
//			console.log('EmbellishResource', 'resource', resource)
//		}).catch(error => {
//			console.log('EmbellishResource', 'resourceId', resourceId)
//		})
//}

function TokenizeTextarea(textarea) {
    var buffer = []
    const parts = textarea.split(/(\s+)/)
    for (const part of parts) {
        console.log('part', part)

        const trimPart = part.trim()
        console.log('trimPart.length', trimPart.length)

        if (trimPart.length > 0) {
            buffer.push(trimPart)
        }
    }
    return buffer
}

function IsURL(str) {
    var pattern = new RegExp('^(https?:\\/\\/)?' + // protocol
        '((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|' + // domain name
        '((\\d{1,3}\\.){3}\\d{1,3}))' + // OR ip (v4) address
        '(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*' + // port and path
        '(\\?[;&a-z\\d%_.~+=-]*)?' + // query string
        '(\\#[-a-z\\d_]*)?$', 'i'); // fragment locator
    return !!pattern.test(str);
}

async function DetermineURLTokens(tokens) {
    var buffer = []
    for (const token of tokens) {
        const isUrl = IsURL(token)
        console.log('DetermineURLTokens', token, isUrl)

				if (isUrl) {
        console.log('DetermineURLTokens', 'inside')
					var resource = null
					try {
						resource = await objects.Resource.findOne({
							url: token,
						})
						console.log('DetermineURLTokens', 'resource', resource)
					} catch (error) {
							console.log('DetermineURLTokens', 'error', error)
					}

					if (!resource) {
						resource = new objects.Resource({
								_id: new mongoose.Types.ObjectId(),
								url: token,
						})
						await resource.save()
					}

					if (resource)  {
						// EmbellishResource(resource)
						const resourceId = resource.id
						buffer.push({
								token,
								resourceId,
						})
					} else {
						buffer.push({
								token,
						})
					}
				} else {
					buffer.push({
							token,
					})
				}

    }
    return buffer
}

function ExtractUrls(embellishedTokens) {
	console.log('ExtractUrls')
    for (const token of embellishedTokens) {
	console.log('ExtractUrls', 'token', token)
        if (token.resourceId) {
					resources.ScrapeMetatagsFromUrl(token.token).then(result => {
							console.log('ExtractUrls', 'result', result)
							console.log('ExtractUrls', 'result', JSON.stringify(result))
					}).catch(error => {
							console.log('ExtractUrls', 'error', error)
					})
        }
    }
}

function CountUrlTokens(embellishedTokens) {
    var buffer = []
    var count = 0
    for (const token of embellishedTokens) {
        if (token.isUrl) {
            count += 1
        }
    }
    return count
}


async function DoMain() {
		console.log('hi1')
		const tokens = TokenizeTextarea(textarea)
		console.log('tokens', tokens)
		console.log('hi2')

		const embellishedTokens = await DetermineURLTokens(tokens)
		console.log('embellishedTokens', embellishedTokens)

	ExtractUrls(embellishedTokens)

		console.log('hi3')
		const numUrls = CountUrlTokens(embellishedTokens)
		console.log('numUrls', numUrls)

		console.log('hi4')

		const basicData = {
				userId,
				tokens: embellishedTokens,
		}
	console.log('basicData', basicData)
		console.log('hi5')
	console.log('why')
        mongoose.connection.close()
}

DoMain()

async function CreateBroadcast(textarea, producerTuple) {
