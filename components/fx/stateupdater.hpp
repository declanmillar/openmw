#ifndef OPENMW_COMPONENTS_FX_STATEUPDATER_H
#define OPENMW_COMPONENTS_FX_STATEUPDATER_H

#include <osg/BufferTemplate>

#include <components/sceneutil/statesetupdater.hpp>
#include <components/std140/ubo.hpp>

namespace fx
{
    class StateUpdater : public SceneUtil::StateSetUpdater
    {
    public:
        StateUpdater(bool useUBO);

        void setProjectionMatrix(const osg::Matrixf& matrix)
        {
            mData.get<ProjectionMatrix>() = matrix;
            mData.get<InvProjectionMatrix>() = osg::Matrixf::inverse(matrix);
        }

        void setViewMatrix(const osg::Matrixf& matrix) { mData.get<ViewMatrix>() = matrix; }

        void setInvViewMatrix(const osg::Matrixf& matrix) { mData.get<InvViewMatrix>() = matrix; }

        void setPrevViewMatrix(const osg::Matrixf& matrix) { mData.get<PrevViewMatrix>() = matrix;}

        void setEyePos(const osg::Vec3f& pos) { mData.get<EyePos>() = osg::Vec4f(pos, 0.f); }

        void setEyeVec(const osg::Vec3f& vec) { mData.get<EyeVec>() = osg::Vec4f(vec, 0.f); }

        void setFogColor(const osg::Vec4f& color) { mData.get<FogColor>() = color; }

        void setSunColor(const osg::Vec4f& color) { mData.get<SunColor>() = color; }

        void setSunPos(const osg::Vec4f& pos) { mData.get<SunPos>() = pos; }

        void setResolution(const osg::Vec2f& size) {
            mData.get<Resolution>() = size;
            mData.get<RcpResolution>() = {1.f / size.x(), 1.f / size.y()};
        }

        void setSunVis(float vis) {
            mData.get<SunVis>() = vis;
            if (vis <= 0.f)
                mData.get<SunPos>().z() *= -1.f;
        }

        void setFogRange(float near, float far) {
            mData.get<FogNear>() = near;
            mData.get<FogFar>() = far;
        }

        void setNearFar(float near, float far) {
            mData.get<Near>() = near;
            mData.get<Far>() = far;
        }

        void setIsUnderwater(bool underwater) { mData.get<IsUnderwater>() = underwater; }

        void setIsInterior(bool interior) { mData.get<IsInterior>() = interior; }

        void setFov(float fov) { mData.get<Fov>() = fov; }

        void setGameHour(float hour) { mData.get<GameHour>() = hour / 24.f; }

        void setWaterHeight(float height) { mData.get<WaterHeight>() = height; }

        void setSimulationTime(float time) { mData.get<SimulationTime>() = time; }

        void setDeltaSimulationTime(float time) { mData.get<DeltaSimulationTime>() = time; }

        static std::string getStructDefinition()
        {
            static std::string definition = {};

            if (definition.empty())
            {
                UniformData data;
                definition = data.getDefinition("_omw_data");
            }
            return definition;
        }

    private:
        struct ProjectionMatrix : std140::Field<std140::std140_mat4> {
            static constexpr std::string_view sName = "projectionMatrix";
        };

        struct InvProjectionMatrix : std140::Field<std140::std140_mat4> {
            static constexpr std::string_view sName = "invProjectionMatrix";
        };

        struct ViewMatrix : std140::Field<std140::std140_mat4> {
            static constexpr std::string_view sName = "viewMatrix";
        };

        struct PrevViewMatrix : std140::Field<std140::std140_mat4> {
            static constexpr std::string_view sName = "prevViewMatrix";
        };

        struct InvViewMatrix : std140::Field<std140::std140_mat4> {
            static constexpr std::string_view sName = "invViewMatrix";
        };

        struct EyePos : std140::Field<std140::std140_vec4> {
            static constexpr std::string_view sName = "eyePos";
        };

        struct EyeVec : std140::Field<std140::std140_vec4> {
            static constexpr std::string_view sName = "eyeVec";
        };

        struct FogColor : std140::Field<std140::std140_vec4> {
            static constexpr std::string_view sName = "fogColor";
        };

        struct SunColor : std140::Field<std140::std140_vec4> {
            static constexpr std::string_view sName = "sunColor";
        };

        struct SunPos : std140::Field<std140::std140_vec4> {
            static constexpr std::string_view sName = "sunPos";
        };

        struct Resolution : std140::Field<std140::std140_vec2> {
            static constexpr std::string_view sName = "resolution";
        };

        struct RcpResolution : std140::Field<std140::std140_vec2> {
            static constexpr std::string_view sName = "rcpResolution";
        };

        struct FogNear : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "fogNear";
        };

        struct FogFar : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "fogFar";
        };

        struct Near : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "near";
        };

        struct Far : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "far";
        };

        struct Fov : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "fov";
        };

        struct GameHour : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "gameHour";
        };

        struct SunVis : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "sunVis";
        };

        struct WaterHeight : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "waterHeight";
        };

        struct IsUnderwater : std140::Field<std140::std140_bool> {
            static constexpr std::string_view sName = "isUnderwater";
        };

        struct IsInterior : std140::Field<std140::std140_bool> {
            static constexpr std::string_view sName = "isInterior";
        };

        struct SimulationTime : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "simulationTime";
        };

        struct DeltaSimulationTime : std140::Field<std140::std140_float> {
            static constexpr std::string_view sName = "deltaSimulationTime";
        };

        using UniformData = std140::UBO<
            ProjectionMatrix,
            InvProjectionMatrix,
            ViewMatrix,
            PrevViewMatrix,
            InvViewMatrix,
            EyePos,
            EyeVec,
            FogColor,
            SunColor,
            SunPos,
            Resolution,
            RcpResolution,
            FogNear,
            FogFar,
            Near,
            Far,
            Fov,
            GameHour,
            SunVis,
            WaterHeight,
            IsUnderwater,
            IsInterior,
            SimulationTime,
            DeltaSimulationTime
        >;

    private:
        void setDefaults(osg::StateSet* stateset) override;
        void apply(osg::StateSet* stateset, osg::NodeVisitor* nv) override;

        UniformData mData;
        bool mUseUBO;
    };
}

#endif