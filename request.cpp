#include "./request.hpp"


Request::Request(){
    std::cout << "i am here" << std::endl;

}
Request::Request(std::string req){
this->vec = split(req,"\r\n");
    for (size_t i = 0; i < vec.size(); i++)
    {
        std::cout << "*********************"<< vec[i]<< "**************************" << std::endl;
    }
}