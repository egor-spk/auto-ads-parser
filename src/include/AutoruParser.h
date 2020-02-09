#ifndef AUTO_ADS_PARSER_AUTORUPARSER_H
#define AUTO_ADS_PARSER_AUTORUPARSER_H

#include "IParser.h"
#include <Document.h>

class AutoruParser : public IParser
{
public:
    /**
     * @param link строка должна содержать поля в формате fmtlib для указания страницы
     */
    explicit AutoruParser(const char *link);

    std::vector<Ad> &getAds() noexcept override;

    void parse() override;

    size_t countResult() const noexcept override;

    void clean() noexcept override;

private:
    static size_t getPageCount(CDocument &doc);

    void parsePageAds(CDocument &doc);

    static std::string getLink(CNode &node);

    static std::string getId(const std::string &link);

    static uint32_t getPrice(CNode &node);

    static uint16_t getYear(CNode &node);

    static uint32_t getMileage(CNode &node);

    std::vector<Ad> ads_;
    const char *link_;
};


#endif //AUTO_ADS_PARSER_AUTORUPARSER_H
