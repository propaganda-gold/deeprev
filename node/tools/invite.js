const objects = require("../models/objects")
const emailAuthorization = require("../util/emailAuthorization")

async function CreateEmailToken(email) {
    console.log('CreateEmailToken', email)
    const emailToken = new objects.EmailToken({
        _id: new mongoose.Types.ObjectId(),
        email,
    })

    emailToken.save().then(function(result) {
        console.log('result', result)
        mongoose.connection.close()

        const token = result._id
        const url = "http://localhost:8080/invitation/accept/" + token
        console.log('url', url)

    }).catch(function(error) {
        console.log('error', error)
        mongoose.connection.close()
    })
}

async function CreateEmailAuthorization(email) {
    console.log('CreateEmailAuthorization', email)
    emailAuthorization.CreateEmailAuthorization(email).then(function(result) {
			CreateEmailToken(email)
    }).catch(function(error) {
        console.log('error', error)
        mongoose.connection.close()
    })
}

const email = process.argv[2]
if (email == null) {
    console.log('email is null')
    process.exit(1)
}

console.log('process.argv', process.argv)

const mongoose = require("mongoose")

const uri = process.env.DEEPREV_MONGOOSE_URI
console.log('mongoose uri', uri)
mongoose.connect(uri, {
    useNewUrlParser: true,
    useUnifiedTopology: true
})

CreateEmailAuthorization(email)
