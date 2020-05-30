#include "fake_twitter/utils.h"

using namespace fake_twitter::utils;
/* submodule cryptopp
#include "sha.h"
#include "filters.h"
#include "base64.h"

std::string SHA256HashString(std::string aString){
    std::string digest;
    CryptoPP::SHA256 hash;

    CryptoPP::StringSource foo(aString, true,
                               new CryptoPP::HashFilter(hash,
                                                        new CryptoPP::Base64Encoder (
                                                                new CryptoPP::StringSink(digest))));

    return digest;
}
*/

// TODO:add MAC and IP address(or use uuid)
std::string fake_twitter::utils::salt() {
    auto now = std::chrono::system_clock::now();
    return date::format("%FT%TZ", now);
}
/*#include <crypt.h>
fake_twitter::PasswordHash fake_twitter::utils::make_password_hash(
    const std::string& salt, const std::string& password) {
    // TODO: proper hashing. std::hash is not consistent between runs
    //    return std::hash<std::string>{}(salt + password);
    const char *p = password.c_str();
    const char *s = salt.c_str();
    std::string pas = crypt(p, s);
    //std::cout << pas;
    delete [] p;
    delete [] s;
    //auto tohash = salt + password;
    //return std::accumulate(tohash.begin(), tohash.end(), 0);
    return pas;
}
*/
fake_twitter::PasswordHash fake_twitter::utils::make_password_hash(
    const std::string& salt, const std::string& password) {
    // TODO: proper hashing. std::hash is not consistent between runs
    //    return std::hash<std::string>{}(salt + password);
    auto tohash = salt + password;
    return std::accumulate(tohash.begin(), tohash.end(), 0);
}
