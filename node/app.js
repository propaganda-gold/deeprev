const express = require("express")
const app = express()
const BodyParser = require("body-parser");
const cors = require('cors')


app.use(BodyParser.urlencoded({ extended: false }))
app.use(BodyParser.json())
app.use(cors())

app.use("/broadcast/create", require("./api/broadcast/create"))
app.use("/broadcast/read", require("./api/broadcast/read"))

app.use("/cookietoken/create", require("./api/cookietoken/create"))
app.use("/cookietoken/validate", require("./api/cookietoken/validate"))
app.use("/cookietoken/delete", require("./api/cookietoken/delete"))

app.use("/emailtoken/validate", require("./api/emailtoken/validate"))
app.use("/emailtoken/create", require("./api/emailtoken/create"))

app.use("/user/create", require("./api/user/create"))
app.use("/user/update", require("./api/user/update"))
app.use("/user/read", require("./api/user/read"))

app.use("/feed/single", require("./api/feed/single"))

app.use("/label/update", require("./api/label/update"))


// Error handling
app.use((request, response, next) => {
  const error = new Error("Not found")
  error.status = 404
  next(error)
})

app.use((error, request, response, next) => {
  response.status(error.status || 500)
  response.json({
    error: {
      message: error.message
    }
  })
})

module.exports = app
