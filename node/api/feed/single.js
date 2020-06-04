const express = require("express")
const redis = require("redis")
const router = express.Router()
const objects = require("../../models/objects")
const login = require("../../util/login")
const hotRandom = require("../../util/hotRandom")

async function FetchAndSendDocument(broadcastId, response) {
    objects.Broadcast.findById(broadcastId).then(broadcast => {
			if (broadcast) {
        response.status(200).json({
            success: true,
            broadcast,
        })
			} else {
        response.status(200).json({
            success: false,
            objectIsNull: true,
        })
			}
    }).catch(error => {
        response.status(200).json({
            success: false,
            error,
        })
    })
}

async function PickARandomDocument(response) {
    console.log('PickARandomDocument')
    hotRandom.PickRandomDocument().then(documentId => {
        console.log('PickARandomDocument', 'documentId', documentId)
        FetchAndSendDocument(documentId, response)
    }).catch(error => {
        console.log('PickARandomDocument', 'error', error)
        response.status(200).json({
            success: false,
            error,
        })
    })
}


router.post("", login.Required, (request, response, next) => {
    PickARandomDocument(response)
})

module.exports = router
