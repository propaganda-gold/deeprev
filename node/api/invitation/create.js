const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")

async function CreateInvitation(request, response) {
    console.log('CreateInvitation', request.body)
    const options = request.body
    try {
        const invitation = new objects.Invitation({
            candidateEmail: options.email,
            sponsorId: options.sponsor,
            creationTime: Date.now(),
        })
        const invitationResult = await Invitation.insertOne(invitation)
        console.log('invitationResult', invitationResult)

        const updateResult = await objects.User.updateOne(
            { _id: sponsorId },
            { $push: { invitations: invitation._id } })
        console.log('updateResult', updateResult)
        
        response.status(200).json({
            invitationResult,
            updateResult,
        })
    } catch (error) {
        response.status(500).json({
            error
        })
    }
}

router.post("create", (request, response, next) => {
    CreateInvitation(request, response)
})

module.exports = router

