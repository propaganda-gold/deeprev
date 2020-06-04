const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const login = require("../../util/login")

function MaskUser(fullUser) {
    return {
        _id: fullUser._id,
        userName: fullUser.userName,
        profile: fullUser.profile,
        description: fullUser.description,
    }
}

async function ReadUser(userName, response) {
    console.log('ReadUser', userName)
    objects.User.findOne({
        userName
    }).then(user => {
        const userView = MaskUser(user)
        response.status(200).json({
            success: true,
            userView,
        })
    }).catch(error => {
        response.status(200).json({
            success: false,
            error,
        })
    })
}

async function CheckArguments(request, response) {
    // Step 1: Try get username from arguments.
    const bodyName = request.body.userName
    const cookieName = request.authenticatedUser ?
        request.authenticatedUser.userName : null
    if (bodyName) {
        ReadUser(userName, response)
    } else if (cookieName) {
        ReadUser(cookieName, response)
    } else {
        response.status(200).json({
            success: false,
            userNotLoggedIn: true,
        })
    }
}



router.post("", login.Required, (request, response, next) => {
    CheckArguments(request, response)
})

module.exports = router
