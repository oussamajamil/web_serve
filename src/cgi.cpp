#include "../include/cgi.hpp"


Cgi::Cgi() {};
Cgi::~Cgi() {};

void Cgi::_envMap(Request req, std::string file)
{
	this->env_map["REDIRECT_STATUS"] = "200";
    this->env_map["QUERY_STRING"]=req.query_params;
    this->env_map["CONTENT_TYPE"]=req.headers.find("Content-Type")->second;
    ///////
    // this->env_map["SERVER_NAME"]="";
	// this->env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
    ///////
    this->env_map["SERVER_PORT"]=req.port+"";
    this->env_map["CONTENT_LENGTH"]=req.headers.find("Content-Length")->second!="" ? req.headers.find("Content-Length")->second : "";
    this->env_map["REMOTE_HOST"]=req.host;
    ///////
    // this->env_map["REQUEST_METHOD"]=req.method;
    // std::cout << "|" << req.method << "|" << std::endl;
    ////////
    this->env_map["SCRIPT_NAME"]=file;
    this->env_map["PATH_INFO"]=req.is_directory_file.second.substr(req.root.length());
    this->env_map["PATH_TRANSLATED"]=req.is_directory_file.second.substr(req.root.length());
    this->env_map["AUTH_TYPE"] = "Basic";
    this->env_map["REMOTE_USER"] = req.headers.find("From")->second !="" ? req.headers.find("From")->second : "";
    this->env_map["SERVER_PROTOCOL"] = req.version;
    // this->env_map["SERVER_SOFTWARE"] = "webserv/1.0";
    this->env_map["HTTP_ACCEPT"] = req.headers.find("Accept")->second!="" ? req.headers.find("Accept")->second : "";
    this->env_map["HTTP_COOKIE"] = req.headers.find("Cookie")->second!="" ? req.headers.find("Cookie")->second : "";
    this->env_map["HTTP_ACCEPT_LANGUAGE"] = req.headers.find("Accept-Language")->second!="" ? req.headers.find("Accept-Language")->second : "";
    this->env_map["HTTP_REFERER"] = req.headers.find("Referer")->second!= "" ? req.headers.find("Referer")->second : "";
    this->env_map["HTTP_USER_AGENT"] = req.headers.find("User-Agent")->second !="" ? req.headers.find("User-Agent")->second : "";
    this->env_map["HTTP_ACCEPT_ENCODING"] = req.headers.find("Accept-Encoding")->second !="" ? req.headers.find("Accept-Encoding")->second : "";
    this->env_map["HTTP_ACCEPT_CHARSET"] = req.headers.find("Accept-Charset")->second !="" ? req.headers.find("Accept-Charset")->second : "";
    this->env_map["DOCUMENT_ROOT"] = req.root;
    this->env_map["REQUEST_URI"] = req.path + req.query_params != "" ? "?" + req.query_params : "";
    this->env_map["UPLOAD_PATH"] = req._server.__attributes["upload_dir"][0] !="" ? req._server.__attributes["upload_dir"][0] : "";
}

int Cgi::execute(Request req, std::string cgi_filePath, std::string file)
{
    std::cout << "execute request body" << req.body << std::endl;
    std::string result = "";
    _envMap(req, file);
    setEnv();
    char *av[3];
    av[0] = (char*)cgi_filePath.c_str();
    av[1] = (char*)file.c_str();
    av[2] = NULL;
    FILE *fIn = tmpfile();
    FILE *fOut = tmpfile();
    int fdIn = fileno(fIn);
    int fdOut = fileno(fOut);
    
    write(fdIn, req.body.c_str(), req.body.size());
    char buff[100000];
    read(fdIn, buff, 100000);
    std::cout << "buff -------: " << buff << std::endl;
    lseek(fdIn, 0, SEEK_SET);
    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(fdIn, STDIN_FILENO);
        dup2(fdOut, STDOUT_FILENO);
        execve(av[0], (char **)av, _env);
        write(fdOut, "error 500", 5);
    }
    else
    {
        waitpid(pid, NULL, 0);
        lseek(fdOut, 0, SEEK_SET);
        char buf[1024];
        int n;
        while ((n = read(fdOut, buf, 1024)) > 0)
            result += std::string(buf, n);
        std::cout << "---------------------------------------------------------------" << std::endl;
        this->results = result;
        std::cout << "---------------------------------------------------------------" << std::endl;
    }
    fclose(fIn);
    fclose(fOut);

    for (int i = 0; i < allocSize; i++)
        delete[] _env[i];
    // for (int i = 0; i <3 ; i++)
    //     delete[] av[i];
    return 0;
}

std::string Cgi::_getInput(std::string file)
{
  std::string body = "";
  char c;
  std::ifstream ifs;
  ifs.open(file);
  if (!ifs)
  {
    std::cout << "file open error\n";
  }
  else
  {
    while (ifs.get(c))
      body += c;
  }
  ifs.close();
  return body;
}

void Cgi::setEnv()
{
    allocSize = env_map.size() + 1;
    _env = new char *[allocSize];
    std::map<std::string, std::string>::iterator itMap = env_map.begin();
    int iEnv = 0;
    while (itMap != env_map.end())
    {
        std::string temp = itMap->first + "=" + itMap->second;
        _env[iEnv] = new char[temp.size() + 1];
        for (unsigned long i = 0; i < temp.size(); i++)
            _env[iEnv][i] = temp[i];
        _env[iEnv][temp.size()] = 0;
        iEnv++;
        itMap++;       
    }
    _env[iEnv] = NULL;
 }
