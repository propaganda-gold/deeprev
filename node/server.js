// const express = require('express')
// const app = express()
const app = require("./app");
const port = 3000
const mongoose = require("mongoose")

const uri = process.env.DEEPREV_MONGOOSE_URI
console.log('mongoose uri', uri)
mongoose.connect(uri, {useNewUrlParser: true,  useUnifiedTopology: true})

app.listen(port, () => console.log(`Example app listening on port ${port}!`))
