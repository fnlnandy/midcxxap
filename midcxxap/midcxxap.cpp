#include "midcxxap.hpp"

MidCxxAP::Argument::Argument(const std::string &posArgName)
{
    m_isPositional = true;
    m_isRequired = true;
    m_short = posArgName;
    m_long = {};

    if (__startswith(m_short, "-"))
        __error("positional argument cannot start with a dash: \"" + m_short + "\".");
}

MidCxxAP::Argument::Argument(const std::string &shortArg, const std::string &longArg, const bool isRequired)
{
    m_isPositional = false;
    m_isRequired = isRequired;
    m_short = shortArg;
    m_long = longArg;

    if (m_short.empty() || __startswith(m_short, "--") || !__startswith(m_short, "-"))
        __error("short argument shouldn't be empty, and should start with a single dash '-': \"" + m_short + "\".");
    if (m_long.empty() || !__startswith(m_long, "--"))
        __error("long argument shouldn't be empty, and should start with a double dash '--': \"" + m_long + "\".");
}

std::string MidCxxAP::Argument::getArgName() const
{
    if (m_isPositional)
        return m_short;

    return m_long.substr(2);
}

MidCxxAP::ArgumentParser::ArgumentParser(const std::string &progName, const std::string &progDesc)
{
    m_progName = progName;
    m_progDesc = progDesc;

    m_posArgs = {};
    m_stdArgs = {};
    m_parsedArgs = {};
}

MidCxxAP::Argument &MidCxxAP::ArgumentParser::addArgument(const std::string &posArgName)
{
    m_posArgs.emplace_back(posArgName);
    m_posArgsString += posArgName + " ";

    return m_posArgs.back();
}

MidCxxAP::Argument &MidCxxAP::ArgumentParser::addArgument(const std::string &shortArg, const std::string &longArg, const bool isRequired)
{
    m_stdArgs.emplace_back(shortArg, longArg, isRequired);
    m_stdArgsString += shortArg + "/" + longArg + " ";

    return m_stdArgs.back();
}

std::unordered_map<std::string, std::string> &MidCxxAP ::ArgumentParser::parseArgs(const int argc, char **const argv)
{
    std::vector<std::string> rawArgs = {argv, argv + argc};
    uint posId = {0};
    bool parseNextArgument = false;
    Argument *parsingArg = nullptr;

    for (const std::string &currentArg : rawArgs)
    {
        if (parseNextArgument && parsingArg != nullptr)
        {
            std::string argName = parsingArg->getArgName();

            parsingArg->setAbsent(false);
            m_parsedArgs[argName] = currentArg;
            parseNextArgument = false;
        }
        else if (!__startswith(currentArg, "-"))
        {
            if (posId >= m_posArgs.size())
                __error("excess positional argument provided: \"" + currentArg + "\".");

            std::string argName = m_posArgs.at(posId).getArgName();

            if (__keyexists(m_parsedArgs, argName))
                __warning("duplicate argument name found for: \"" + currentArg + "\".");

            m_posArgs.at(posId).setAbsent(false);
            m_parsedArgs[argName] = currentArg;
            posId++;
        }
        else if (__startswith(currentArg, "--"))
        {
            tryToParseNonPositional(currentArg, parseNextArgument, &parsingArg, false);
        }
        else if (__startswith(currentArg, "-"))
        {
            tryToParseNonPositional(currentArg, parseNextArgument, &parsingArg, true);
        }
        else
        {
            __error("unexpected argument: \"" + currentArg + "\".");
        }
    }

    tryToPrintUsage();
    return m_parsedArgs;
}

MidCxxAP::Argument *MidCxxAP::ArgumentParser::findMatchingArg(const std::string &s, bool isShort)
{
    for (Argument &arg : m_stdArgs)
    {
        if (isShort)
        {
            if (arg.getShort() == tryToSplitArg(s))
                return &arg;
        }
        else
        {
            if (tryToSplitArg(arg.getLong()) == tryToSplitArg(s))
                return &arg;
        }
    }

    return nullptr;
}

std::string MidCxxAP::ArgumentParser::tryToSplitArg(const std::string &s, bool returnArg) const
{
    std::string::size_type idx = s.rfind('=');

    // If '=' is not found, or otherwise if it's the last character.
    if ((idx == std::string::npos || idx == s.length() - 1) && returnArg)
    {
        return {};
    }
    if (returnArg)
    {
        // Since idx contains the index of the '=',
        // and we're supposed to return the arg.
        return s.substr(idx + 1);
    }

    return s.substr(0, idx);
}

void MidCxxAP::ArgumentParser::tryToParseNonPositional(const std::string &currentArg, bool &parseNextArgument, MidCxxAP::Argument **parsingArg, bool isShort)
{
    Argument *foundArg = findMatchingArg(currentArg, isShort);

    if (foundArg == nullptr)
    {
        __warning("unexpected argument found: \"" + currentArg + "\".");
        return;
    }

    std::string argName = foundArg->getArgName();

    if (__keyexists(m_parsedArgs, argName))
        __warning("duplicate argument name found for: \"" + currentArg + "\".");

    std::string argValue = tryToSplitArg(currentArg, true);

    if (argValue.length() == 0)
    {
        parseNextArgument = true;
        *parsingArg = foundArg;
        return;
    }

    foundArg->setAbsent(false);
    m_parsedArgs[argName] = argValue;
}

void MidCxxAP::ArgumentParser::tryToPrintUsage(void) const
{
    std::string missingPosArgsString = {};
    std::string missingStdArgsString = {};

    for (const Argument &arg : m_posArgs)
        if (arg.getAbsent())
            missingPosArgsString += arg.getArgName() + " ";
    for (const Argument &arg : m_stdArgs)
        if (arg.getAbsent())
            missingStdArgsString += arg.getShort() + "/" + arg.getLong() + " ";

    if (missingPosArgsString.length() == 0 && missingStdArgsString.length() == 0)
        return;

    std::printf("%s: \"%s\"\n"
                "\033[34m\033[1mUsage:\033[0m %s %s%s\n"
                "\033[31m\033[1mMissing positional arguments:\033[0m %s\n"
                "\033[31m\033[1mMissing standard arguments:\033[0m %s\n",
                m_progName.c_str(), m_progDesc.c_str(),
                m_progName.c_str(), m_posArgsString.c_str(), m_stdArgsString.c_str(),
                missingPosArgsString.c_str(),
                missingStdArgsString.c_str());
    std::exit(1);
}