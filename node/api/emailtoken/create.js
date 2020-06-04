const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const mongoose = require('mongoose')
const emailAuthorization = require("../../util/emailAuthorization")

async function FinishCreateEmailToken(response, email) {
    console.log('DoCreateEmailToken', 'email', email)
    const emailtoken = new objects.EmailToken({
        _id: new mongoose.Types.ObjectId(),
        email,
    })

    emailtoken.save().then(function(result) {
        const token = result._id
        console.log('DoCreateEmailToken', 'result', result)
        console.log('DoCreateEmailToken', 'token', token)
        const url = process.env.DEEPREV_VUE_BASE_PATH + "/login/cookietoken/" + token
        console.log('DoCreateEmailToken', 'url', url)

        response.status(200).json({
            success: true,
            reason: 'Check your email.',
        })

    }).catch(function(error) {
        console.log('DoCreateEmailToken', 'error', error)

        response.status(200).json({
            success: false,
            error,
        })

    })
}

async function CheckEmailIsAuthorized(response, email) {
    console.log('DoCreateEmailToken', 'email', email)
    emailAuthorization.CheckEmailAuthorization(email).then(result => {
        if (result) {
            console.log('CheckEmailIsAuthorized', 'result', result)
            FinishCreateEmailToken(response, email)
        } else {
            response.status(200).json({
                success: false,
                emailNotAuthorized: true,
            })
        }
    }).catch(error => {
        response.status(200).json({
            success: false,
            error,
        })
    })
}

async function CheckEmailArgumentPresent(request, response) {
    console.log('CheckEmailArgumentPresent', 'body', request.body)
    if (request.body && request.body.email) {
        const email = request.body.email
        console.log('CheckEmailArgumentPresent', 'email', email)
        CheckEmailIsAuthorized(response, email)
    } else {
        response.status(200).json({
            success: false,
            reason: 'No email in request.',
        })
    }
}

router.post("", (request, response, next) => {
    console.log('CheckEmailArgumentPresent', 'body', request.body)
    CheckEmailArgumentPresent(request, response)
})

module.exports = router
