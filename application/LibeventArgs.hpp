#ifndef YAD_LIBEVENTARGS_HPP
#define YAD_LIBEVENTARGS_HPP
#include "QueryResponder.hpp"

<<<<<<< HEAD
<<<<<<< HEAD
///class to provide callback function for
///OnRequest2 function
=======
>>>>>>> fc06ffd (YAS-12-add-application-template-class)
=======
///class to provide callback function for
///OnRequest2 function
>>>>>>> ba6314f (YAS-12-add-application-template-class)
struct LibeventArgs {
    QueryResponder *responder;
    std::function <void(void)> stop_callback;
};
#endif //YAD_LIBEVENTARGS_HPP
