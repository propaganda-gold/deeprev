const objects = require("../models/objects")
const resources = require("../util/resources")
const mongoose = require("mongoose")
const cheerio = require('cheerio');
const getUrls = require('get-urls');
const fetch = require('node-fetch');

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
  var pattern = new RegExp('^(https?:\\/\\/)?'+ // protocol
    '((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|'+ // domain name
    '((\\d{1,3}\\.){3}\\d{1,3}))'+ // OR ip (v4) address
    '(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*'+ // port and path
    '(\\?[;&a-z\\d%_.~+=-]*)?'+ // query string
    '(\\#[-a-z\\d_]*)?$','i'); // fragment locator
  return !!pattern.test(str);
}

function EmbellishTokensWithLinks(tokens) {
	for (const token of tokens) {
		const isUrl = IsURL(token)
		console.log('EmbellishTokensWithLinks', token, usUrl)
	}
}

// Loops through each element of 'tokens'.
// If it is a url, look this up in the database, and add the value to a resource.
// @async returns a Promise; has to wait for the network.
async function SerializeResources(tokens) {
	console.log('SerializeResources', 'tokens', tokens)
	var promiseBuffer = []
	var error_ = null
	for (const token of tokens) {
		const isUrl = IsURL(token)
		console.log('SerializeResources', 'isUrl', isUrl, token)
		if (isUrl) {
			// Note: we have checked that 'token' is a url
			const promise = resources.BlockingResouceFromUrl(token)
			promiseBuffer.push(promise)
		}
	}

	try {
		console.log('a')
		var resourceBuffer = []
		console.log('a')
		for (var promise of promiseBuffer) {
		console.log('a1')
			// Note: await
			console.log('SerializeResources', 'typeof promise', typeof promise)
			const resource = await promise
		console.log('a2')
			console.log('SerializeResources', 'resource', resource)
		console.log('a')
			resourceBuffer.push(resource)
		console.log('a')
		}
		console.log('a')
	} catch(error) {
		console.log('SerializeResources', 'error', error)
		error_ = error
	}

	console.log('SerializeResources', 'resourceBuffer', resourceBuffer)
	return new Promise(function (resolve, reject) {
		if (error_) {
			reject(error_)
		} else {
			resolve(resourceBuffer)
		}
	})
}

// producerTuple has elements {'_id', 'userName'}
async function CreateBroadcast(textarea, producerTuple) {
	var broadcastResult_ = null
	var error_ = null
	try {
		const tokens = TokenizeTextarea(textarea)
		const resources = await SerializeResources(tokens)
		const producerId = producerTuple._id
		const producerName = producerTuple.userName
		const creationTime = 0
		const broadcast = new objects.Broadcast({
			_id: new mongoose.Types.ObjectId(),
			textarea,
			tokens,
			resources,
			producerTuple,
			creationTime,
		})
		console.log('CreateBroadcast', 'broadcast', broadcast)

		// TODO(greg) Can this be done without await?
		broadcastResult_ = await broadcast.save()
	} catch (error) {
		error_ = error
	}
	return new Promise(function(resolve, reject) {
		if (error_) {
			reject(error_)
		} else {
			resolve(broadcastResult_)
		}
	})
}

module.exports = {
    CreateBroadcast,
}

