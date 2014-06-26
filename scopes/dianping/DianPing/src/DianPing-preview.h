#ifndef DEMOPREVIEW_H
#define DEMOPREVIEW_H

#include<unity/scopes/PreviewQueryBase.h>

class DianpingPreview : public unity::scopes::PreviewQueryBase
{
public:
    DianpingPreview(std::string const& uri);
    ~DianpingPreview();

    virtual void cancelled() override;
    virtual void run(unity::scopes::PreviewReplyProxy const& reply) override;

private:
    std::string uri_;
};

#endif
