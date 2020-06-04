const mongoose = require("mongoose")
const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")

async function SendEmailToUser(user, emailtoken, response) {
    const link = 'http://localhost:8080/user/create/' + emailtoken
    console.log('SendEmailToUser', user, emailtoken, link)
    response.status(200).json({
        link
    })
}

async function CreateEmailToken(user, response) {
    console.log('SendEmailToUser', user)
    const emailtoken = RandomToken(18)
    objects.User.updateOne( {_id : user.id}, {$set : {emailtoken}}).then(function(result) {
        SendEmailToUser(user, emailtoken, response)
    }).catch( function(error) {

    })
}


async function DoCreateToken(params, response) {
    console.log('DoCreateToken', params)
    objects.User.find( { email: params.email } ).then(function(user) {
        CreateEmailToken(user, response)
    }).catch(function(error) {
        response.status(500).json({
            error
        })
    })
}

router.post("create", (request, response, next) => {
    const params = {
        email: request.body.email,
    }
    DoCreateToken(params, response)
})

async function DoValidateLoginToken(request, response) {
    const logintoken = request.params.token
    objects.LoginToken.findById(logintoken).then(function(result) {

    }).catch(function(error) {
        response.status(500).json({
            error,
        })
    })
    const userId = emailtoken.CheckToken(emailtoken)
    if (userId != null) {
        response.status(200).json({
            success: true
        })
    } else {
        response.status(200).json({
            success: false
        })
    }
}

router.post("validate", (request, response, next) => {
    DoValidateLoginToken(request, response)
})

module.exports = router
