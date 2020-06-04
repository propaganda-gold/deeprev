const mongoose = require("mongoose")

const resources = require("../util/resources")


const uri = process.env.DEEPREV_MONGOOSE_URI
console.log('mongoose uri', uri)
mongoose.connect(uri, {
    useNewUrlParser: true,
    useUnifiedTopology: true
})

const url = process.argv[2].trim()
if (url == null) {
    console.log('url is null')
    process.exit(1)
}

resources.BlockingResouceFromUrl(url).then(result => {
	console.log('Final result:', result)
}).catch(error => {
	console.log('error', error)
})
