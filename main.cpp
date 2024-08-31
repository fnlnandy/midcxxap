#include <iostream>
#include <midcxxap/midcxxap.hpp>

int main(int argc, char **argv)
{
    auto argumentParser = MidCxxAP::ArgumentParser("testmidcxxap", "test program for the midcxxap library.");

    argumentParser.addArgument("filename");
    argumentParser.addArgument("-s", "--source", true);
    argumentParser.addArgument("-d", "--destination", true);

    auto parsedArgs = argumentParser.parseArgs(argc - 1, argv + 1);

    std::printf("FILENAME: %s SOURCE: %s DESTINATION: %s\n", parsedArgs["filename"].c_str(), parsedArgs["source"].c_str(), parsedArgs["destination"].c_str());
    return 0;
}