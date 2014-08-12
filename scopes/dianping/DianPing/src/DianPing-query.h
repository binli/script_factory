#include <unity/scopes/SearchQueryBase.h>
#include <unity/scopes/ReplyProxyFwd.h>

class DianpingQuery : public unity::scopes::SearchQueryBase
{
public:
    DianpingQuery(std::string const& query);
    ~DianpingQuery();
    virtual void cancelled() override;

    virtual void run(unity::scopes::SearchReplyProxy const& reply) override;

private:
    std::string query_;
};
