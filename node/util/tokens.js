// This file contains the *utility* functions for accessing the in-memory
// Redis table "cookietoken_map".
//
// This is NOT the authentication middleware. This will be called by the
// middleware.
const config = require("./config")

console.log('redisConfig', config)
console.log('redisConfig', config.redisConfig)

const redis = require("redis")
const redisClient = redis.createClient(config.redisConfig)
redisClient.on("error", function(error) {
    console.error(error)
})

function RandomToken(length) {
    var result = '';
    var characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    var charactersLength = characters.length;
    for (var i = 0; i < length; i++) {
        result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }
    return result;
}

function AuthenticatedUser(user) {
    console.log('AuthenticatedUser', 'user', user)
    return {
        _id: user._id.toString(),
        userName: user.userName,
    }
}

// Returns a Promise.
async function ResetCookieToken(user) {
    console.log('ResetCookieToken', 'user', user)
    const cookietoken = RandomToken(24)
    console.log('ResetCookieToken cookietoken', cookietoken)
    const condensedUser = AuthenticatedUser(user)
    console.log('ResetCookieToken', 'condensedUser', condensedUser)
    const jsonUser = JSON.stringify(condensedUser)

    return new Promise(function(resolve, reject) {
        redisClient.hset("cookietoken_map", cookietoken, jsonUser, err => {
            if (err) {
                reject(err)
            } else {
                resolve(cookietoken)
            }
        })
    })
}

async function FindCookieTokenUser(cookietoken) {
    console.log('FindCookieTokenUser', 'cookietoken', cookietoken)
    return new Promise(function(resolve, reject) {
        redisClient.hget("cookietoken_map", cookietoken, function(error, userRep) {
            console.log('FindCookieTokenUser', 'hget', error, userRep)
            if (error) {
                reject(error)
            } else {
                if (userRep) {
                    resolve(JSON.parse(userRep))
                } else {
                    reject({
                        loginFailed: true,
                    })
                }
            }
        })
    })
}

async function ClearCookieToken(cookietoken) {
    console.log('ClearCookieToken', 'cookietoken', cookietoken)
    return new Promise(function(resolve, reject) {
        console.log('ClearCookieToken', 'promise')
        redisClient.hdel("cookietoken_map", cookietoken, function(error, result) {
            console.log('ClearCookieToken', 'error', error)
            if (error) {
                reject(error)
            } else {
                resolve(result)
            }
        })
    })
}

module.exports = {
    ResetCookieToken,
    FindCookieTokenUser,
    ClearCookieToken,
}