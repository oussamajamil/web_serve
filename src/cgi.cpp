#include "../include/cgi.hpp"

const std::string Cgi::env_list[DEFAULT] = 
{"AUTH_TYPE","CONTENT_LENGTH","CONTENT_TYPE","GATEWAY_INTERFACE",
"PATH_INFO","PATH_TRANSLATED","QUERY_STRING","REMOTE_ADDR","REMOTE_IDENT",
"REMOTE_USER","REQUEST_METHOD","REQUEST_URI","SCRIPT_FILENAME","SERVER_NAME",
"SERVER_PORT","SERVER_PROTOCOL","SERVER_SOFTWARE","REDIRECT_STATUS"};

Cgi::Cgi()
{}

Cgi::~Cgi(void)
{}

/*
    funct execute ==> execute a CGI program 
            1: Request => object that contain information about HTTP request
            2: cgiFilePath => the path to be executed 
            3: file => the file to be used as input for the CGI program
*/

int Cgi::execute(Request req, std::string cgiFilePath, std::string file) 
{
    std::string reslt = "";
    if (cgiFilePath.find("php-cgi") == std::string::npos) // find php_cgi is not exist  
    {
        cgiInput = req.body;
    }
    else    
        cgiInput = getInput(file);
        
    std::string cfp = cgiFilePath;
    setEnviron(req, file);
    char *tmp = new char [cfp.size() + 1];
    for (unsigned long i = 0; i < cfp.size(); i++)
        tmp[i] = cfp[i];
    tmp[cfp.size()] = 0;
    char *av[] = {tmp, NULL};
    FILE *fIn = tmpfile();
    FILE *fOut = tmpfile();
    long fdIn = fileno(fIn);
    long fdOut = fileno(fOut);
    write(fdIn, cgiInput.c_str(), cgiInput.size());\
    lseek(fdIn, 0, SEEK_SET);
    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(fdIn, STDIN_FILENO);
        dup2(fdOut, STDOUT_FILENO);
        execve(cgiFilePath.c_str(), av, env);
    }
    else
    {
        const size_t buffSize = 1048;
        char buff[buffSize] = {};
        wait(NULL);
        lseek(fdOut, 0, SEEK_SET);
        ssize_t bytes = 0;
        while ((bytes = read(fdOut, buff, 1024)) > 0)
        {
            buff[bytes] = 0;
            std::string temp = buff;
            reslt += temp;
            memset(buff, 0, sizeof(buff));
        }
        if (bytes == -1)
            return -1;
    }
    std::vector<std::string> v;
    std::stringstream ss;
    ss << reslt;
    while(!ss.eof())
    {
        std::string l;
        std::getline(ss, l);
        v.push_back(l);
    }
    setCgiRespoHeader(v);
    setCgiRespoBody(v);
    for(size_t i = 0; i < alloc_size; i++)
        delete[] env[i];
    delete[] env;
    delete[] tmp;
    return 0;
}

std::map<std::string, std::string> Cgi::env_Map(Request &req, std::string file)
{
    std::map<std::string, std::string> test;
    test[env_list[REQUEST_METHOD]] = req.method;
    test[env_list[SCRIPT_FILENAME]] = file;
    test[env_list[PATH_INFO]] = req.is_directory_file.second.substr(req.root.length());
    test[env_list[REQUEST_URI]] = req.path + req.query_params != "" ? "?" + req.query_params : "" ;
    test[env_list[REDIRECT_STATUS]] = "200";
    test[env_list[SERVER_PROTOCOL]] = req.version;
    test[env_list[CONTENT_TYPE]] = req.headers.find("Content-Type")->second;
    test[env_list[GATEWAY_INTERFACE]] = "CGI/1.1";
    test[env_list[REMOTE_ADDR]] = "127.0.0.1";
    test[env_list[SERVER_PORT]] = req.port + "";
    test[env_list[SERVER_SOFTWARE]] = "webserv/1.0";
    test[env_list[PATH_TRANSLATED]] = req.is_directory_file.second.substr(req.root.length());
    test[env_list[CONTENT_LENGTH]] = req.headers.find("Content-Length")->second!="" ? req.headers.find("Content-Length")->second : "";
    return test;
}

void Cgi::setEnviron(Request &req, std::string file)
{
    std::map<std::string, std::string> envMap = env_Map(req, file);
    alloc_size = envMap.size() + 1;
    env = new char *[alloc_size];
    std::map<std::string, std::string>::iterator itMap = envMap.begin();
    size_t iEnv = 0;
    while (itMap != envMap.end())
    {
        std::string temp = itMap->first + "=" + itMap->second;
        env[iEnv] = new char[temp.size() + 1];
        for (unsigned char i = 0; i < temp.size(); i++)
            env[iEnv][temp.size()] = 0;
            iEnv++;
            itMap++;
    }
    env[iEnv] = NULL;
}

std::string Cgi::getInput(std::string file)
{
    std::string body = "";
    char c;
    std::ifstream ifs;
    ifs.open(file);
    if (!ifs)
        std::cout << "file open error \n";
    else
    {
        while (ifs.get(c))
            body += c;
    }
    ifs.close();
    return body;
}

void Cgi::setCgiRespoHeader(std::vector<std::string> &v)
{
    if (v.size() >= 2)
        cgiRespoHeader = v[0] + '\n' + v[1];
}

std::string Cgi::getCgiRespoHeader()
{
    return cgiRespoHeader;
}

void Cgi::setCgiRespoBody(std::vector<std::string> &v)
{
    std::string temp;
    for (size_t i = 2; i < v.size(); i++)
    {
        temp += v[i];
        if (i != v.size() - 1)
            temp += '\n';
    }
    cgiRespoBody = temp;
}

std::string Cgi::getCgiRespoBody()
{
    return cgiRespoBody;
}