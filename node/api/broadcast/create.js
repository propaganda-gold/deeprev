const mongoose = require("mongoose")
const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const tokenchecker = require('../../util/tokenchecker')
const tokens = require('../../util/tokens')
const login = require("../../util/login")
const createBroadcast = require('../../util/createBroadcast')
const hotRandom = require('../../util/hotRandom')

async function CallCreateBroadcast(textarea, producerTuple, response) {
    console.log('CallCreateBroadcast', textarea, producerTuple)
    createBroadcast.CreateBroadcast(textarea, producerTuple).then(broadcastResult => {
        console.log('broadcastResult', broadcastResult)

        hotRandom.AddDocument(broadcastResult._id).then(addResult => {
            console.log('main', 'addResult', addResult)
            response.status(200).json({
                success: true,
                broadcast: broadcastResult,
            })
        }).catch(error => {
            response.status(200).json({
                success: false,
                error,
            })
        })

    }).catch(error => {
        response.status(200).json({
            success: false,
            error,
        })
    })
}

async function CheckPreconditions(request, response) {
    console.log('CheckPreconditions', 'request.body', request.body)
    const textarea = request.body.textarea
    const producerTuple = request.authenticatedUser
    console.log('CheckPreconditions', 'textarea', textarea)
    console.log('CheckPreconditions', 'producerTuple', producerTuple)
    if (!textarea) {
        response.status(200).json({
            success: false,
            error: "textAreaIsNull",
        })
    } else if (!producerTuple) {
        response.status(200).json({
            success: false,
            error: "notLoggedIn",
        })
    } else {
        CallCreateBroadcast(textarea, producerTuple, response)
    }
}

router.post("", login.Required, (request, response, next) => {
    CheckPreconditions(request, response)
})

module.exports = router
