const mongoose = require("mongoose");

const UserSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    userName: {
        type: String,
        required: true
    },
    email: {
        type: String,
        required: true
    },
    description: {
        type: String,
        default: ''
    },
    profile: {
        type: String,
        default: ''
    },
    pictureFileName: {
        type: String,
        default: ''
    },
})
const User = mongoose.model("User", UserSchema)

const InvitationSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    candidateEmail: {
        type: String,
        required: true
    },
    sponsorId: {
        type: String,
        required: true
    },
    creationTime: {
        type: Number,
        required: true
    },
})
const Invitation = mongoose.model("Invitation", InvitationSchema)

const ImageSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    userId: {
        type: String,
        required: true
    },
    fileInfo: {
        type: String,
        required: true
    },
    creationTime: {
        type: Number,
        required: true
    },
})
const Image = mongoose.model("Image", ImageSchema)

const EmailTokenSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    email: {
        type: String,
        required: true
    },
})
const EmailToken = mongoose.model("EmailToken", EmailTokenSchema)

const UserTokenSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    userId: {
        type: String,
        required: true
    },
})
const CookieToken = mongoose.model("CookieToken", UserTokenSchema)

const EmailAuthorizationSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    email: {
        type: String,
        required: true
    },
})
const EmailAuthorization = mongoose.model("EmailAuthorization", EmailAuthorizationSchema)

const ResourceSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    url: {
        type: String,
        required: true
    },
    title: {
        type: String,
        required: false
    },
    favicon: {
        type: String,
        required: false
    },
    description: {
        type: String,
        required: false
    },
    image: {
        type: String,
        required: false
    },
    author: {
        type: String,
        required: false
    },
})
const Resource = mongoose.model("Resource", ResourceSchema)

const BroadcastSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    textarea: {
        type: String,
        required: true
    },
    tokens: {
        type: [String],
        required: true
    },
    resources: {
        type: [mongoose.Schema.Types.Mixed],
        required: true
    },
    producerTuple:  {
        _id: {
            type: String,
            required: true
        },
        userName: {
            type: String,
            required: true
        },
    },
    creationTime: {
        type: Number,
        required: true
    },
})
const Broadcast = mongoose.model("Broadcast", BroadcastSchema)

const LabelSchema = mongoose.Schema({
    _id: mongoose.Schema.Types.ObjectId,
    // Id of the user that makes the label.
    subjectId: {
        type: String,
        required: true
    },
    // Id of the 'object' being labeled, either a Broadcast or a User.
    objectId: {
        type: String,
        required: true
    },
    // Type of the 'object' being labeled, either a Broadcast or a User.
    objectType: {
        type: String,
        required: true
    },
    // The name of the variable that we are giving a value for.
    labelName: {
        type: String,
        required: true
    },
    // The value that we are giving to the variable.
    labelValue: {
        type: String,
        required: true
    },
    // Time this was created.
    creationTime: {
        type: Number,
        required: true
    },
})
const Label = mongoose.model("Label", LabelSchema)

module.exports = {
    User,
    Invitation,
    Image,
    EmailToken,
    CookieToken,
    EmailAuthorization,
    Resource,
    Broadcast,
    Label,
}
