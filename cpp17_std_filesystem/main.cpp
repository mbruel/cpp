#include <string>
#include <iostream>
#include <iomanip>    // std::setprecision
#include <experimental/filesystem> // C++-standard header file name
#include <fstream>
#include <regex>
#include <algorithm>  // std::replace
namespace fs = std::experimental::filesystem;

static const std::regex  sTargetExtension("^\\.(rar|r\\d+)$");
static const char *sOutputFileName   = "rarSize.txt";
static const char *sTemplateFileName = "template.txt";
static const std::string sSizeMarkup = "__SIZE__";
static std::string sOutputContent;

#define USE_COMA_FLOATS 1

bool loadTemplate(const fs::path &templatePath);
void addRarSizeInFolder(const fs::path &folderPath);
std::string formatSize(unsigned int size);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: you should provide exactly one parameter that should be a folder path" << std::endl;
        return 1;
    }

    fs::path folderPath(argv[1]);
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath))
    {
        std::cerr << "Error: the given parameter is not an existing folder" << std::endl;
        return 2;
    }

    fs::path appPath(argv[0]);
    if (!loadTemplate(appPath.parent_path() / sTemplateFileName))
    {
        std::cerr << "Error: couldn't read template file: " << sTemplateFileName << std::endl;
        return 3;
    }

    addRarSizeInFolder(fs::absolute(folderPath));
    return 0;
}

void addRarSizeInFolder(const fs::path &folderPath)
{
    unsigned int size = 0;
    std::cout << "Scanning folder: " << folderPath.string() << std::endl;
    for (const auto & entry : fs::directory_iterator(folderPath))
    {
        const fs::path &path =entry.path();
        const auto filename = path.filename().string();
        if (fs::is_directory(entry.path()))
                addRarSizeInFolder(fs::absolute(entry.path()));
        else if (std::regex_match(path.extension().string(), sTargetExtension))
        {
            size += fs::file_size(path);
            std::cout << "\t- " << path.filename().string() << std::endl;
        }
        else
            std::cout << "\t- IGNORE " << path.filename().string() << " (ext: " << path.extension().string() << ")" <<std::endl;
    }

    if (size > 0)
    {
        std::string rarSize = formatSize(size);
        std::cout << "=> " << folderPath << " rar size: " << rarSize << std::endl;

        fs::path outputFile = folderPath / sOutputFileName;
        std::ofstream output(outputFile.string());
        if (output.is_open())
        {
            std::string s = sOutputContent;
            s.replace(s.find(sSizeMarkup), sSizeMarkup.length(), rarSize);
            output << s;
            output.close();
        }
        else
            std::cout << "Error: can't write output in folder: " << outputFile.string() << std::endl;
    }
}

bool loadTemplate(const fs::path &templatePath)
{
    std::ifstream istream(templatePath.string());
    if (istream.is_open())
    {
        std::stringstream buffer;
        buffer << istream.rdbuf();
        sOutputContent = buffer.str();
        return true;
    }
    else
        return false;
}

std::string formatSize(unsigned int size)
{
    std::string unit("B");
    double s = static_cast<double>(size);
    if (s > 1024)
    {
        unit = "kB";
        s /= 1024.;
        if (s > 1024)
        {
            unit = "MB";
            s /= 1024.;
            if (s > 1024)
            {
                unit = "GB";
                s /= 1024.;
            }
        }
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << s << " " << unit;

#ifdef USE_COMA_FLOATS
    std::string str = ss.str();
    std::replace(str.begin(), str.end(), '.', ',');
    return str;
#else
    return ss.str();
#endif
}


/*
// QT Version
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#define USE_COMA_FLOATS 1

bool loadTemplate();
int addRarSizeInFolder(const QString &folderPath);
QString formatSize(qint64 size);

static QRegularExpression sTargetExtension("^(rar|r\\d+)$");
static const QString      sOutputFileName   = "rarSize.txt";
static const QString      sTemplateFileName = "template.txt";
static       QString      sOutputContent;
static QTextStream cout(stdout);
static QTextStream cerr(stderr);



int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Error: you should provide exactly one parameter that should be a folder path\n";
        return 1;
    }

    QFileInfo folderInf(argv[1]);
    if (!folderInf.exists() || !folderInf.isDir())
    {
        cerr << "Error: the given parameter is not an existing folder\n";
        return 2;
    }

    if (!loadTemplate())
    {
        cerr << "Error: couldn't read template file: " << sTemplateFileName << endl;
        return 3;
    }

    addRarSizeInFolder(folderInf.absoluteFilePath());
    return 0;
}


int addRarSizeInFolder(const QString &folderPath)
{
    QDir dir(folderPath);
    qint64 size = 0;
    cout << "Scanning folder: " << folderPath << endl;
    for (const QFileInfo &path : dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries, QDir::Name|QDir::DirsLast))
    {
        if (path.isDir())
            addRarSizeInFolder(path.absoluteFilePath());
        else if (sTargetExtension.match(path.suffix()).hasMatch())
        {
            size += path.size();
            cout << "\t- " << path.fileName() << ", size: " << path.size() << endl;
        }
    }
    if (size > 0)
    {
        QString rarSize = formatSize(size);
        cout << "=> " << folderPath << " rar size: " << rarSize << endl;

        QString outputFileName = QString("%1/%2").arg(folderPath).arg(sOutputFileName);
        QFile file(outputFileName);
        if (file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << sOutputContent.arg(rarSize);
            file.close();
        }
        else
        {
            cout << "Error: can't write output in folder: " << outputFileName << endl;
            return 1;
        }
    }
    return 0;
}

QString formatSize(qint64 size)
{
    QString unit("B");
    double s = static_cast<double>(size);
    if (s > 1024)
    {
        unit = "kB";
        s /= 1024.;
        if (s > 1024)
        {
            unit = "MB";
            s /= 1024.;
            if (s > 1024)
            {
                unit = "GB";
                s /= 1024.;
            }
        }
    }
#ifdef USE_COMA_FLOATS
    return QString("%1 %2").arg(s, 0, 'f', 2).arg(unit).replace(".", ",");
#else
    return QString("%1 %2").arg(s, 0, 'f', 2).arg(unit).replace(".", ",");
#endif
}

bool loadTemplate()
{
    QFile templateFile(sTemplateFileName);
    if (templateFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream stream(&templateFile);
        sOutputContent = stream.readAll();
        templateFile.close();
        return true;
    }
    else
        return false;
}
*/
