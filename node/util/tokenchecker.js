// const redis = require("redis")

// class TokenChecker {
//     constructor(tableName) {
//         this.tableName = tableName
//         this.client = redis.createClient();

//         client.on("error", function (error) {
//             console.error(error);
//         })

//     }

//     RecoverUserOrNull(emailtoken) {
//         return new Promise(function (resolve, reject) {
//             this.client.hget(map_name, function (err, replies) {
//                 if (err) {
//                     reject(err)
//                 } else {
//                     resolve(replies)
//                 }
//             })
//         })
//     }

//     SetUserToken(userId, emailtoken) {
//         return new Promise(function (resolve, reject) {
//             this.client.hset(map_name, function (err, replies) {
//                 if (err) {
//                     reject(err)
//                 } else {
//                     resolve()
//                 }
//             })
//         })
//     }
// }

// const staticEmailtokenChecker = new TokenChecker('emailtoken')
// const staticCookietokenChecker = new TokenChecker('cookietoken')

// export default {
//     staticEmailtokenChecker,
//     staticCookietokenChecker,
// }
