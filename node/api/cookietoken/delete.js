const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const tokens = require('../../util/tokens')

async function DeleteCookieToken(cookietoken, response) {
	console.log('DeleteCookieToken', 'cookietoken', cookietoken)
		tokens.ClearCookieToken(cookietoken).then(result => {
			console.log('DeleteCookieToken', 'result', result)
			response.status(200).json({
					success: true
			})
		}).catch(error => {
			response.status(200).json({
					success: false,
					error,
			})
		})
}

async function CheckCookieToken(request, response) {
	if (request.body && request.body.cookietoken) {
		const cookietoken = request.body.cookietoken
		console.log('CheckCookieToken', 'cookietoken', cookietoken)
		tokens.FindCookieTokenUser(cookietoken).then(userId => {
			if (cookietoken) {
				DeleteCookieToken(cookietoken, response)
			} else {
				response.status(200).json({
						success: false,
						error: "cookietoken not found",
				})
			}
		}).catch(error => {
			response.status(200).json({
					success: false,
					error,
			})
		})
	} else {
		console.log('CheckCookieToken', 'request.userId not found')
			response.status(200).json({
					success: false,
					error: "Arguments missing",
			})
	}
}

router.post("", (request, response, next) => {
    CheckCookieToken(request, response)
})

module.exports = router


