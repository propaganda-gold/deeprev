const redisConfig = {
    host: '18.237.116.77',
}

const redis = require("redis")
console.log('redisConfig', redisConfig)

const redisClient = redis.createClient(redisConfig)

redisClient.on("error", function(error) {
    console.error(error)
  })

redisClient.on('connect', () => {
  console.log('redisClient.connected in', redisClient.connected)

  redisClient.set("test", "hi2", function(error) {
    console.log('error', error)
  })
})


  const getMethods = (obj) => {
    let properties = new Set()
    let currentObj = obj
    do {
      Object.getOwnPropertyNames(currentObj).map(item => properties.add(item))
    } while ((currentObj = Object.getPrototypeOf(currentObj)))
    return [...properties.keys()].filter(item => typeof obj[item] === 'function')
  }

  console.log('getMethods', getMethods(redisClient))


  redisClient.info(function(e) {
    console.log('info', e)
  })


  console.log('redisClient.connected out', redisClient.connected)
