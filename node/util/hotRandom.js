const objects = require('../models/objects')
const config = require("./config")
const redis = require("redis")
console.log('redisConfig', config.redisConfig)
const redisClient = redis.createClient(config.redisConfig)
redisClient.on("error", function(error) {
    console.error(error)
})

async function AddDocument(documentId) {
    const idString = documentId.toString()
    return new Promise(function(resolve, reject) {
        if (!documentId) {
            reject({
                internalError: 'documentId is null'
            })
        }
        redisClient.sadd("hot_document_set", idString, function(error, result) {
            if (error) {
                reject(error)
            } else {
                resolve(result)
						}
        })
    })
}

async function PickRandomDocument() {
    return new Promise(function(resolve, reject) {
        redisClient.srandmember("hot_document_set", function(error, broadcastId) {
            if (error) {
                reject(error)
            }
            if (!broadcastId) {
                reject({
                    error: 'broadcastId null',
                })
            }
            resolve(broadcastId)
        })
    })
}


module.exports = {
    AddDocument,
    PickRandomDocument,
}
