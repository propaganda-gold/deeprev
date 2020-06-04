const objects = require("../models/objects")
const mongoose = require("mongoose")

const uri = process.env.DEEPREV_MONGOOSE_URI
console.log('mongoose uri', uri)
mongoose.connect(uri, {useNewUrlParser: true,  useUnifiedTopology: true})

const userName = process.argv[3]
if (userName == null) {
    console.log('userName is null')
    process.exit(1)
}

const userName = process.argv[3]
if (userName == null) {
    console.log('userName is null')
    process.exit(1)
}

objects.User.find({userName}).exec().then(function(result) {
    console.log('result', result)
    mongoose.connection.close()
}).catch(function(error) {
    console.log('error', error)
    mongoose.connection.close()

})
