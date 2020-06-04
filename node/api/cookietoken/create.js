const mongoose = require("mongoose")
const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const tokenchecker = require('../../util/tokenchecker')
const tokens = require('../../util/tokens')

async function UserNotRegistered(email, response) {
    console.log('UserNotRegistered', 'email', email)
    response.status(200).json({
        success: false,
        emailNotRegistered: true,
        email,
    })
}

async function LookupUserByEmail(email, response) {
    objects.User.findOne({
        email
    }).then(function(userObject) {
        console.log('LookupUserByEmail', 'userObject', userObject)
        if (userObject) {
					console.log('LookupUserByEmail', 'branch 1')
            tokens.ResetCookieToken(userObject).then(cookietoken => {
							response.status(200).json({
									success: true,
									cookietoken,
							})
						}).catch(error => {
							response.status(200).json({
									success: false,
									error,
							})
						})
        } else {
						console.log('LookupUserByEmail', 'branch 2')
            UserNotRegistered(email, response)
        }
    }).catch(function(error) {
					console.log('LookupUserByEmail', 'error', error)
        response.status(200).json({
            success: false,
            error,
        })
    })
}

// 'request' must contain a positional 'emailtoken'
async function CheckEmailToken(request, response) {
    console.log('CheckEmailToken', request.body)
    const emailtoken = request.body.emailtoken
    if (!emailtoken) {
        response.status(200).json({
            success: false,
            error: "'emailtoken' is null",
        })
    } else {
        objects.EmailToken.findById(emailtoken).then(function(emailtokenObject) {
            LookupUserByEmail(emailtokenObject.email, response)
        }).catch(function(error) {
            response.status(200).json({
                success: false,
                error,
            })
        })
    }
}

router.post("", (request, response, next) => {
    console.log('authtoken create')
    CheckEmailToken(request, response)
})

module.exports = router
