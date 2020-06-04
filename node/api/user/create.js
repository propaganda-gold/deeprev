const mongoose = require("mongoose")
const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const tokenchecker = require('../../util/tokenchecker')
const tokens = require('../../util/tokens')

async function RespondWithCookieToken(user, response) {
		console.log('RespondWithCookieToken', 'user', user)
    tokens.ResetCookieToken(user._id).then(cookietoken => {
			console.log('RespondWithCookieToken', 'cookietoken', cookietoken)
        response.status(200).json({
            success: true,
            user,
            cookietoken,
        })
    }).catch(error => {
			console.log('RespondWithCookieToken', 'error', error)
        response.status(200).json({
            success: false,
            error,
        })
    })
}

async function ActuallyCreateTheUser(args, email, response) {
	console.log('ActuallyCreateTheUser', 'args', args)
    const user = objects.User({
        _id: new mongoose.Types.ObjectId(),
        email,
        userName: args.userName,
				description: args.description,
				profile: args.profile,
    })
    user.save().then(function(result) {
        RespondWithCookieToken(user, response)
    }).catch(function(error) {
        console.log('DoCreateEmailToken', 'error', error)
        response.status(200).json({
            success: false,
            error,
        })
    })
}

async function RetrieveEmailFromToken(args, response) {
	console.log('RetrieveEmailFromToken', 'args', args)
    objects.EmailToken.findOne({
        _id: args.emailtoken
    }).then(result => {
			console.log('RetrieveEmailFromToken', 'result', result)
        if (result) {
            ActuallyCreateTheUser(args, result.email, response)
        } else {
            response.status(200).json({
                success: false,
                error: "no emailtoken found",
            })
        }
    }).catch(error => {
	console.log('RetrieveEmailFromToken', 'error', error)
        response.status(200).json({
            success: false,
            error,
        })
    })
}

async function CheckIfEmailTaken(args, response) {
    console.log('CheckIfEmailTaken', 'args', args)
    objects.User.findOne({
        email: args.email
    }).then(result => {
			console.log('CheckIfEmailTaken', 'result', result)
        if (result) {
            response.status(200).json({
                success: false,
                emailTaken: true,
                result,
            })
        } else {
            RetrieveEmailFromToken(args, response)
        }
    }).catch(error => {
        response.status(200).json({
            success: false,
            error,
        })
    })
}

async function CheckIfUserNameTaken(args, response) {
    console.log('CheckIfUserNameTaken', 'args', args)
    objects.User.findOne({
        userName: args.userName
    }).then(result => {
			console.log('CheckIfUserNameTaken', 'result', result)
        if (result) {
            response.status(200).json({
                success: false,
                userNameTaken: true,
                result,
            })
        } else {
            RetrieveEmailFromToken(args, response)
        }
    }).catch(error => {
        response.status(200).json({
            success: false,
            error,
        })
    })
}

router.post("", (request, response, next) => {
    CheckIfUserNameTaken(request.body, response)
})

module.exports = router
