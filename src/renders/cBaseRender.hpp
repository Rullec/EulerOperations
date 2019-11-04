#include <memory>
#include <vector>

class cBaseRenderObj;

class cBaseRender{
public:
    cBaseRender(std::string conf_path);
    ~cBaseRender();
    virtual void Init();
    virtual void Update();
private:
    void ConstructMembers(std::string );
    void ParseConf();
    void InitShader();
    std::string mConfPath, mVertexShaderPath, mFragmentShaderPath;
    unsigned int mShaderProgram;
    std::vector<std::unique_ptr<cBaseRenderObj>> mRenderList;
};