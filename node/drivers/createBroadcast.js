const cheerio = require('cheerio');
const getUrls = require('get-urls');
const fetch = require('node-fetch');

const objects = require("../models/objects")
const resources = require("../util/resources")
const hotRandom = require("../util/hotRandom")
const createBroadcast = require("../util/createBroadcast")

const mongoose = require("mongoose")

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

const producerName = process.argv[3]
if (producerName == null) {
    console.log('producerName is null')
    process.exit(1)
}

objects.User.findOne({userName: producerName}).then(userResult => {
	console.log({userResult})
	createBroadcast.CreateBroadcast(textarea, userResult).then(broadcastResult => {
		console.log('broadcastResult', broadcastResult)

		hotRandom.AddDocument(broadcastResult._id).then(addResult => {
			console.log('main', 'addResult', addResult)
		}).catch(addError => {
			console.log('main', 'addError', addError)
		})

	}).catch(error => {
		console.log('error', error)
	})
}).catch(error => {
		console.log('error', error)
})

