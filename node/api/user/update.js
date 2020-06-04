const mongoose = require("mongoose")
const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")

async function DoUpdateProperty(property, request, response) {
    console.log('DoUpdateProperty', request.body)
    var update = {}
    update[property] = request.body.value
    objects.User.updateOne({ "_id": tuple.id }, update).then(
        function (result) {
            response.status(200).json({
                result
            })
        }
    ).catch(
        function (error) {
            response.status(500).json({
                error
            })
        })
}

router.post("caption", (request, response, next) => {
    DoUpdateProperty("caption", request, response)
})

router.post("profile", (request, response, next) => {
    DoUpdateProperty("profile", request, response)
})

module.exports = router
