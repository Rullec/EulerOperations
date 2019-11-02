#include <memory>
#include <vector>
// #include "RenderObj/cBaseRenderObj.h"

class cBaseRenderObj;

class cBaseShader{
public:
    cBaseShader(std::string, unsigned int );
    ~cBaseShader();
    void ReadSrcFromFile(std::string src_path);
    unsigned int GetShaderHandle();
private:
    void CompileSrc();
    const unsigned int INVALID_HANDLE = INT16_MAX;
    std::string mSrcPath;
    std::string mSrc;
    unsigned int mShaderHandle;
    unsigned int mShaderType;
};

class cBaseRender{
public:
    cBaseRender(std::string conf_path);
    ~cBaseRender();
    virtual void Init();
private:
    void ConstructMembers(std::string );
    std::string mConfPath;
    std::unique_ptr<cBaseShader> mVertexShader, mFragmentShader;
    std::vector<std::unique_ptr<cBaseRenderObj>> mRenderList;
};