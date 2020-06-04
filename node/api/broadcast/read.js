const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")

async function ReadBroadcast(broadcastId, response) {
	objects.Broadcast.findById(broadcastId).then(broadcast => {
		response.status(200).json({
			success: true,
			broadcast,
		})
	}).catch(error => {
		response.status(200).json({
			success: false,
			error,
		})
	})
}

router.post("", (request, response, next) => {
	const broadcastId = request.body.broadcastId
	if (!broadcastId) {
		response.status(200).json({
			success: false,
			error: 'missingArgument',
			errorDetail: 'broadcastId',
		})
	} else {
		ReadBroadcast(broadcastId, response)
	}
})

module.exports = router


