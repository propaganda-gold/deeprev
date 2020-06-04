const mongoose = require('mongoose')
const objects = require("../models/objects")

// Returns a promise.
async function CheckEmailAuthorization(email) {
	return objects.EmailAuthorization.findOne( { email } )
}

// Returns a promise.
async function CreateEmailAuthorization(email) {
    console.log('CreateEmailAuthorization', email)
    const emailApproval = new objects.EmailAuthorization({
        _id: new mongoose.Types.ObjectId(),
        email,
    })

    return emailApproval.save()
}

module.exports = {
    CheckEmailAuthorization,
    CreateEmailAuthorization,
}
