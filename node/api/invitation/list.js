const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")

async function DoListInvitations(request, response) {
    console.log('DoListInvitations', request.body)
    const options = request.body
    try {
        const user = objects.User.findOne( {_id : options.user})
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

router.post("", (request, response, next) => {
    DoListInvitations(request, response)
})

module.exports = router


