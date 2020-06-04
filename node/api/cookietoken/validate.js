const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const tokens = require("../../util/tokens")

async function DoValidateCookieToken(request, response) {
    console.log('DoValidateCookieToken', 'request.body', request.body)
    if (request.body && request.body.cookietoken) {
        const cookietoken = request.body.cookietoken
        console.log('DoValidateCookieToken', 'cookietoken', cookietoken)
        tokens.FindCookieTokenUser(cookietoken).then(authenticatedUser => {
            response.status(200).json({
                success: true,
                authenticatedUser,
            })
				}).catch(error => {
            response.status(200).json({
                success: false,
                error,
            })
        })
    } else {
        console.log('DoValidateCookieToken', 'request.userId not found')
        response.status(200).json({
            success: false,
            error: "Arguments missing",
        })
    }
}

router.post("", (request, response, next) => {
    DoValidateCookieToken(request, response)
})

module.exports = router
