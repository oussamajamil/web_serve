#include "../include/cgi.hpp"

Cgi::Cgi(){};
Cgi::~Cgi(){};

void Cgi::_envMap(Request req, std::string cgi_filePath, std::string file)
{
    this->env_map["SCRIPT_FILENAME"] = file;
    if (req.method == "GET")
        this->env_map["REQUEST_METHOD"] = "GET";
    else
        this->env_map["REQUEST_METHOD"] = "POST";
    if (req.headers.find("Content-Length") != req.headers.end())
        this->env_map["CONTENT_LENGTH"] = req.headers.find("Content-Length")->second != "" ? req.headers.find("Content-Length")->second : "";
    if (req.headers.find("Content-Type") != req.headers.end())
        this->env_map["CONTENT_TYPE"] = req.headers.find("Content-Type")->second != "" ? req.headers.find("Content-Type")->second : "";
    this->env_map["REDIRECT_STATUS"] = "200";
    if (req.query_params != "")
        this->env_map["QUERY_STRING"] = req.query_params;
    if (req.path != "")
    {
        this->env_map["REQUEST_URI"] = req.path;
        this->env_map["PATH_INFO"] = req.path;
    }
    if (req.headers.find("Cookie") != req.headers.end())
        this->env_map["HTTP_COOKIE"] = req.headers.find("Cookie")->second != "" ? req.headers.find("Cookie")->second : "";
    this->env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
    if (req.is_directory_file.second != "")
    this->env_map["PATH_TRANSLATED"] = req.is_directory_file.second.substr(req.root.length());
    if (req.headers.find("Accept") != req.headers.end())
        this->env_map["HTTP_ACCEPT"] = req.headers.find("Accept")->second != "" ? req.headers.find("Accept")->second : "";
    if (req._server.__attributes.find("upload_dir") != req._server.__attributes.end())
     this->env_map["UPLOAD_PATH"] = req._server.__attributes["upload_dir"][0] != "" ? req._server.__attributes["upload_dir"][0] : "";
    if (req.headers.find("Accept-Language") != req.headers.end())
        this->env_map["HTTP_ACCEPT_LANGUAGE"] = req.headers.find("Accept-Language")->second != "" ? req.headers.find("Accept-Language")->second : "";
    if (req.headers.find("Referer") != req.headers.end())
    this->env_map["HTTP_REFERER"] = req.headers.find("Referer")->second != "" ? req.headers.find("Referer")->second : "";

    this->env_map["SERVER_PORT"] = req.port + "";
    this->env_map["AUTH_TYPE"] = "Basic";
    this->env_map["SERVER_SOFTWARE"] = "webserv/1.0";
    this->env_map["REMOTE_HOST"] = req.host;
    this->env_map["SERVER_NAME"] = "";
    this->env_map["SERVER_PROTOCOL"] = req.version;
    if (req.headers.find("User-Agent") != req.headers.end())
    this->env_map["HTTP_USER_AGENT"] = req.headers.find("User-Agent")->second != "" ? req.headers.find("User-Agent")->second : "";
    if (req.headers.find("Accept-Encoding") != req.headers.end())
    this->env_map["HTTP_ACCEPT_ENCODING"] = req.headers.find("Accept-Encoding")->second != "" ? req.headers.find("Accept-Encoding")->second : "";
    this->env_map["DOCUMENT_ROOT"] = req.root;
    this->env_map["HTTP_ACCEPT_CHARSET"] = "";
}

int Cgi::execute(Request req, std::string cgi_filePath, std::string file)
{
    std::string result = "";
    _envMap(req, cgi_filePath, file);
    setEnv();
    char *av[3];
    av[0] = (char *)cgi_filePath.c_str();
    av[1] = (char *)file.c_str();
    av[2] = NULL;
    int stdin_save = dup(STDIN_FILENO);
    int stdout_save = dup(STDOUT_FILENO);
    FILE *fIn = tmpfile();
    FILE *fOut = tmpfile();
    int fdIn = fileno(fIn);
    int fdOut = fileno(fOut);
    write(fdIn, req.body.c_str(), std::atoi(req.headers["Content-Length"].c_str()));
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
        this->results = result;
    }
    dup2(stdin_save, STDIN_FILENO);
    dup2(stdout_save, STDOUT_FILENO);
    fclose(fIn);
    fclose(fOut);
    close(fdIn);
    close(fdOut);

    for (int i = 0; i < allocSize; i++)
        delete[] _env[i];
    delete[] _env;
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
