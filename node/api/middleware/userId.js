const objects = require("../../models/objects")
const config = require("../../util/config")
const redis = require("redis")
console.log('redisConfig', config.redisConfig)
const redisClient = redis.createClient(config.redisConfig)
redisClient.on("error", function(error) {
    console.error(error)
  })

function TryEmbellishUserId(request, response, next) {
	console.log('TryEmbellishUserId', 'request.body', request.body)
	if ('cookietoken' in request.body) {
    const cookietoken = request.body.cookietoken
    console.log('TryEmbellishUserId', 'cookietoken', cookietoken)

    redisClient.hget("cookietoken_map", cookietoken, function (error, userId) {
        console.log('TryEmbellishUserId', 'error', error)
        if (!error) {
					console.log('TryEmbellishUserId', 'there is no error')
					console.log('userId', userId)
					if (userId) {
						console.log('TryEmbellishUserId', 'userId is not null')
						request.userId = userId
					} else {
						console.log('TryEmbellishUserId', 'userId is null')
					}
        } else {
					console.log('TryEmbellishUserId', 'there is an error')
				}
				next()
    })
	} else {
		console.log('TryEmbellishUserId', 'cookietoken not found')
		next()
	}
}

module.exports = TryEmbellishUserId

