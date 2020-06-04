const objects = require("../models/objects")
const mongoose = require("mongoose")

const uniqueName = process.argv[2]
if (uniqueName == null) {
    console.log('uniqueName is null')
    process.exit(1)
}

const uri = process.env.DEEPREV_MONGOOSE_URI
mongoose.connect(uri, { useNewUrlParser: true, useUnifiedTopology: true })

objects.User.findOne({ uniqueName }).then(function (user) {
    const token = new objects.LoginToken({
        _id: new mongoose.Types.ObjectId(),
        userId: user._id,
    })
    token.save().then(function (result) {
        console.log('token result', result)
        mongoose.connection.close()
    }).catch(function (error) {
        console.log('error', error)
        mongoose.connection.close()
    })
}).catch(function (error) {
    console.log('error', error)
    mongoose.connection.close()
})
