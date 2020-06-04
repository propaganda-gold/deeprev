const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")

async function DoValidateEmailToken(request, response) {
    console.log('DoValidateEmailToken', request.params)
    response.status(200).json({
        success: true
    })
}

router.post("", (request, response, next) => {
    DoValidateEmailToken(request, response)
})

module.exports = router

