const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")

async function DoReadImage(request, response) {
    console.log('DoReadImage', request.body)
    const params = request.params
    try {
        const user = objects.Image.findOne( {_id : params.imageId})
        console.log('DoListInvitations', user)
        response.status(200).json({
            user,
        })
    } catch (error) {
        response.status(500).json({
            error,
        })
    }
}

router.post("", upload.single("uploadImage"), (request, response, next) => {
    DoReadImage(request, response)
})

module.exports = router


