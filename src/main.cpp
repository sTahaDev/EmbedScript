#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

class EmbedScript
{

public:
    std::string source;

    enum TokenType
    {
        BinaryOperator,
        OpenParan,
        CloseParan,
        Identifier,
        Number,
        Equals,
        Semicolon,
        Quotes,
        String

    };
    struct Token
    {
        std::string value;
        TokenType type;
    };
    std::vector<Token> Tokens;
    EmbedScript(std::string source)
    {
        this->source = source;
    }

    std::string charToStr(char key)
    {
        return std::string(1, key);
    }
    bool isAlpha(char key)
    {

        return (std::tolower(key) != std::toupper(key));
    }
    bool isInt(char character)
    {
        return std::isdigit(static_cast<unsigned char>(character)) != 0;
    }
    void Lexer()
    {
        char currentSource;
        Token currentToken;
        while (source.size() > 0)
        {

            currentSource = source[0];

            if (currentSource == '#')
            {

                source = source.substr(1);

                while (source.size() > 0 && source[0] != '#')
                {
                    source = source.substr(1);
                }

                source = source.substr(1);
            }
            else if (currentSource == '(')
            {
                currentToken = {charToStr(currentSource), TokenType::OpenParan};
                Tokens.push_back(currentToken);
                source = source.substr(1);
            }
            else if (currentSource == ')')
            {
                currentToken = {charToStr(currentSource), TokenType::CloseParan};
                Tokens.push_back(currentToken);
                source = source.substr(1);
            }
            else if (currentSource == '=')
            {
                currentToken = {charToStr(currentSource), TokenType::Equals};
                Tokens.push_back(currentToken);
                source = source.substr(1);
            }
            else if (currentSource == ';')
            {
                currentToken = {charToStr(currentSource), TokenType::Semicolon};
                Tokens.push_back(currentToken);
                source = source.substr(1);
            }
            else if (currentSource == '+' || currentSource == '-' || currentSource == '*' || currentSource == '%')
            {
                currentToken = {charToStr(currentSource), TokenType::BinaryOperator};
                Tokens.push_back(currentToken);
                source = source.substr(1);
            }
            else if (isInt(source[0]))
            {
                std::string value = "";

                while (isInt(source[0]))
                {
                    value += source[0];
                    source = source.substr(1);
                }
                currentToken = {value, TokenType::Number};
                Tokens.push_back(currentToken);
            }
            else if (currentSource == '"')
            {
                source = source.substr(1);
                std::string value = "";
                while (true)
                {
                    currentSource = source[0];

                    if (!(currentSource == '"'))
                    {
                        value += currentSource;
                        source = source.substr(1);
                    }
                    else
                    {
                        currentToken = {value, TokenType::String};
                        Tokens.push_back(currentToken);
                        source = source.substr(1);
                        break;
                    }
                }
            }
            else if (isAlpha(currentSource))
            {

                std::string ident = "";
                while (isAlpha(source[0]) && source.size() > 0)
                {

                    ident += source[0];
                    source = source.substr(1);
                }

                currentToken = {ident, TokenType::Identifier};
                Tokens.push_back(currentToken);
            }
            else
            {
                source = source.substr(1);
            }
        }
    }

    void Parser()
    {
        size_t index = 0;
        while (index < Tokens.size())
        {
            Token currentToken = Tokens[index];

            // Eğer token bir sayı ise
            if (currentToken.type == TokenType::Number)
            {
                // İleri giderek bir sonraki tokeni al
                index++;
            }
            // Eğer token bir operatör ise
            else if (currentToken.type == TokenType::BinaryOperator)
            {
                // İleri giderek bir sonraki tokeni al
                index++;
                // Operatörden sonra bir sayı olmalı
                if (index >= Tokens.size() || Tokens[index].type != TokenType::Number)
                {
                    std::cerr << "Syntax error: Expected number after operator." << std::endl;
                    return;
                }
                index++;
            }
            // Diğer durumlarda hata ver
            else
            {
                std::cerr << "Syntax error: Unexpected token." << std::endl;
                return;
            }
        }

        std::cout << "Syntax is correct." << std::endl;
    }

    enum VariableTypes
    {
        // Hepsinin Sonunda t var çünki cpp ile karışabilir
        Nullt,
        Stringt,
        Intt,
        Boolt,
        Chart,
        Floatt
    };
    struct
    {
        std::string Let = "let";
    } Identifiers;

    struct Variable
    {
        std::string name;
        std::string value;
        VariableTypes type;

    };

    std::vector<Variable> Variables;

    void Execution()
    {
        Variable currentVar;
        int index = 0;
        while (index < Tokens.size())
        {
            // Deişken Atama
            if (Tokens[index].type == TokenType::Identifier)
            {
                if (Tokens[index].value == Identifiers.Let)
                {

                    index++;
                    if (Tokens[index].type == TokenType::Identifier)
                    {
                        currentVar = {Tokens[index].value,"", VariableTypes::Nullt};
                        Variables.push_back(currentVar);
                        index++;
                    }
                }else{
                    index++;
                }
            }
            else if (Tokens[index].type == TokenType::Equals)
            {
                
                for (int i = 0; i < Variables.size(); i++)
                {
                    if (Variables[i].name == Tokens[index - 1].value)
                    {
                        
                        if (Tokens[index + 1].type == TokenType::Number)
                        {
                            Variables[i].type = VariableTypes::Intt;
                        }
                        else if (Tokens[index + 1].type == TokenType::String)
                        {
                            Variables[i].type = VariableTypes::Stringt;
                        }

                        Variables[i].value = Tokens[index + 1].value;
                    }
                }

                index++;
            }else{
                index++;
            }
        }
    }

    void Run()
    {
        this->Lexer();

        for (int i = 0; i < Tokens.size(); i++)
        {
            std::cout << Tokens[i].value << " - " << Tokens[i].type << std::endl;
        }

        this->Execution();

        for (int i = 0; i < Variables.size(); i++)
        {
            std::cout << "{" << "name: "<< Variables[i].name << ",value: " << Variables[i].value << ", Type: " << Variables[i].type << "}" << std::endl;
        }
    }
};

int main()
{

    std::ifstream inputFile("../src/main.es");

    // Dosyanın başarıyla açılıp açılmadığını kontrol et
    if (!inputFile.is_open())
    {
        std::cerr << "Dosya açma hatası!" << std::endl;
        return 1; // Programı hata ile sonlandır
    }

    // Dosyadan veriyi oku
    std::string line;
    std::string fileRead;
    while (std::getline(inputFile, line))
    {
        fileRead += line;
    }

    EmbedScript script(fileRead);
    script.Run();

    return 0;
}
