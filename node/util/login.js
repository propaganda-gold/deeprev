// This file is for the *middleware* function that adds the user info to the
// request.
// 
const objects = require('../models/objects')
const config = require("./config")
const tokens = require("./tokens")
const redis = require("redis")
console.log('redisConfig', config.redisConfig)
const redisClient = redis.createClient(config.redisConfig)
redisClient.on("error", function(error) {
    console.error(error)
})

function NotRequired(request, response, next) {
    next()
}

function Required(request, response, next) {
    console.log('Required', 'request.body', request.body)
    const cookietoken = request.body.cookietoken
    console.log('Required', 'cookietoken', cookietoken)

    // Short-circuit if cookietoken is null, i.e. don't hit DB.
    if (cookietoken == null) {
        response.status(200).json({
            success: false,
            error: "'cookietoken' is null",
        })
    } else {
        tokens.FindCookieTokenUser(cookietoken).then(userPair => {
            console.log('Required', 'userPair', userPair)
            request.authenticatedUser = userPair
            next()
        }).catch(error => {
            console.log('Required', 'error', error)
            next()
        })
    }
}

module.exports = {
    Required,
    NotRequired,
}
