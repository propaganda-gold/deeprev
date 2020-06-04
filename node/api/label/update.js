const mongoose = require("mongoose")
const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const tokenchecker = require('../../util/tokenchecker')
const tokens = require('../../util/tokens')
const login = require('../../util/login')
const createLabel = require('../../util/createLabel')


async function StartCreateLabel(userId, body, response) {
    console.log('StartCreateLabel', body)
    // TODO(greg) Check the arguments.
    createLabel.CreateLabel(userId, body.objectId, body.objectType, body.labelName, body.labelValue).then(result => {
        response.status(200).json({
            success: true,
            result,
        })
    }).catch(error => {
        response.status(200).json({
            success: false,
            error,
        })
    })

}

router.post("", login.Required, (request, response, next) => {
    if (!request.authenticatedUser) {
        response.status(200).json({
            success: false,
            notLoggedIn: true,
        })
    } else {
        StartCreateLabel(request.authenticatedUser._id, request.body, response)
    }
})

module.exports = router
