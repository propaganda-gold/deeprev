const objects = require("../models/objects")
const mongoose = require("mongoose")

const email = process.argv[2]
if (email == null) {
    console.log('email is null')
    process.exit(1)
}


const uri = process.env.DEEPREV_MONGOOSE_URI
console.log('mongoose uri', uri)
mongoose.connect(uri, {
    useNewUrlParser: true,
    useUnifiedTopology: true
})

async function GetUserByEmail(email) {
	objects.User.findOne({email}).then(result => {
		console.log('GetUserByEmail', 'result', result)
	})
}

GetUserByEmail(email)
