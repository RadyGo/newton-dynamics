/* Copyright (c) <2003-2019> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __D_MESH_EFFECT_H__
#define __D_MESH_EFFECT_H__

#include "dCoreStdafx.h"
#include "dPolyhedra.h"
//#include "dRefCounter.h"

//class dgWorld;
//class dMeshEffect;
//class dgCollisionInstance;

class dMemoryAllocator___;

#define DG_MESH_EFFECT_PRECISION_BITS		48
#define DG_MESH_EFFECT_PRECISION_SCALE		dFloat64(dgInt64(1)<<DG_MESH_EFFECT_PRECISION_BITS)
#define DG_MESH_EFFECT_PRECISION_SCALE_INV	(dFloat64 (1.0f) / DG_MESH_EFFECT_PRECISION_SCALE)

#define DG_VERTEXLIST_INDEXLIST_TOL			(dFloat64 (0.0f))
#define DG_MESH_EFFECT_POINT_SPLITED		512
#define DG_MESH_EFFECT_BVH_STACK_DEPTH		256

//class dMeshEffect: public dPolyhedra, public dgRefCounter
class dMeshEffect : public dPolyhedra
{
#if 0
	public:
	enum dgChannelType
	{
		m_vertex,
		m_normal,
		m_binormal,
		m_uv0,
		m_uv1,
		m_color,
		m_material,
		m_layer,
		m_point,
	};

	class dgMeshVertexFormat
	{
		public:
		class dgDoubleData
		{
			public:
			const dFloat64* m_data;
			const dInt32* m_indexList;
			dInt32 m_strideInBytes;
		};

		class dgFloatData
		{
			public:
			const dFloat32* m_data;
			const dInt32* m_indexList;
			dInt32 m_strideInBytes;
		};

		dgMeshVertexFormat ()
		{
			Clear ();
		}

		void Clear ()
		{
			memset (this, 0, sizeof (dgMeshVertexFormat));
		}

		dInt32 m_faceCount;
		const dInt32* m_faceIndexCount;
		const dInt32* m_faceMaterial;
		dgDoubleData m_vertex;
		dgFloatData m_normal;
		dgFloatData m_binormal;
		dgFloatData m_uv0;
		dgFloatData m_uv1;
		dgFloatData m_vertexColor;
	};

	template<class T, dgChannelType type>
	class dgChannel: public dArray<T>
	{
		public:
		dgChannel(dMemoryAllocator___* const allocator)
			:dArray<T>(allocator)
			,m_count(0)
			,m_type(type)
		{
		}

		dgChannel(const dgChannel& source)
			:dArray<T>(source, source.m_count)
			,m_count(source.m_count)
			,m_type(source.m_type)
		{
		}

		~dgChannel()
		{
		}

		void CopyFrom (const dgChannel<T, type>& source)
		{
			dArray<T>& me = *this;
			dgChannel& src = *((dgChannel*)&source);

			Clear();
			m_count = src.m_count;
			dAssert (m_type == src.m_type);
			for (dInt32 i = 0; i < m_count; i++) {
				me[i] = src[i];
			}
		}

		void Clear()
		{
			m_count = 0;
			dArray<T>::Clear ();
		}

		void Reserve (dInt32 size)
		{
			dArray<T>::Resize(size);
			m_count = size;
		}

		void PushBack (const T& element) 
		{
			T tmp (element);
			dArray<T>& me = *this;
			me[m_count] = tmp;
			m_count ++;
		}

		void SetCount (dInt32 count) 
		{
			if (m_count) {
				dAssert (count >= 0);
				dAssert (m_count >= count);
				m_count = count;
			}
		}

		dInt32 m_count;
		dgChannelType m_type;
	};

	class dgFormat
	{
		public:
		class dgSortKey
		{
			public:
			dInt32 m_mask;
			dInt32 m_ordinal;
			dInt32 m_vertexIndex;
			dInt32 m_attibuteIndex;
		};
		class VertexSortData
		{
			public:
			const dgChannel<dBigVector, m_point>* m_points;
			dInt32 m_vertexSortIndex;
		};

		dInt32 GetSortIndex (const dgChannel<dBigVector, m_point>& points, dFloat64& dist) const;
		static dInt32 CompareVertex(const dgSortKey* const ptr0, const dgSortKey* const ptr1, void* const context);
	};

	class dgPointFormat: public dgFormat
	{
		public:
		dgPointFormat(dMemoryAllocator___* const allocator);
		dgPointFormat(const dgPointFormat& source);
		~dgPointFormat();

		void Clear();
		void SetCount (dInt32 count);
		void CompressData(dInt32* const indexList);

		dgChannel<dInt32, m_layer> m_layers;
		dgChannel <dBigVector, m_point> m_vertex;
	};

	class dgAttibutFormat: public dgFormat
	{
		public:
		class dgUV
		{
			public:
			dFloat32 m_u;
			dFloat32 m_v;
		};

		dgAttibutFormat(dMemoryAllocator___* const allocator);
		dgAttibutFormat(const dgAttibutFormat& source);
		~dgAttibutFormat();

		void Clear();
		void SetCount (dInt32 count);
		void CopyFrom (const dgAttibutFormat& source);
		void CopyEntryFrom (dInt32 index, const dgAttibutFormat& source, dInt32 sourceIndex);
		void CompressData (const dgPointFormat& points, dInt32* const indexList);

		dgChannel<dInt32, m_vertex> m_pointChannel;
		dgChannel<dInt32, m_material> m_materialChannel;
		dgChannel<dTriplex, m_normal> m_normalChannel;
		dgChannel<dTriplex, m_binormal> m_binormalChannel;
		dgChannel<dVector, m_color> m_colorChannel;
		dgChannel<dgUV, m_uv0> m_uv0Channel;
		dgChannel<dgUV, m_uv1> m_uv1Channel;
	};

	class dgIndexArray 
	{
		public:
		dInt32 m_materialCount;
		dInt32 m_indexCount;
		dInt32 m_materials[256];
		dInt32 m_materialsIndexCount[256];
		dInt32* m_indexList;
	};

	class dMeshBVH
	{
		public:
		class dgMeshBVHNode
		{
			public:
			dgMeshBVHNode (const dMeshEffect* const mesh, dEdge* const face, void* const userData);
			dgMeshBVHNode (dgMeshBVHNode* const left, dgMeshBVHNode* const right);
			~dgMeshBVHNode ();
			void SetBox (const dVector& p0, const dVector& p1);

			DG_CLASS_ALLOCATOR(allocator)
			dVector m_p0;
			dVector m_p1;

			dFloat32 m_area;
			dEdge* m_face;
			void* m_userData;
			dgMeshBVHNode* m_left;
			dgMeshBVHNode* m_right;
			dgMeshBVHNode* m_parent;
		};

		class dgFitnessList: public dgTree <dgMeshBVHNode*, dgMeshBVHNode*>
		{
			public:
			dgFitnessList (dMemoryAllocator___* const allocator);
			dFloat64 TotalCost () const;
		};

		
		dMeshBVH (const dMeshEffect* const mesh);
		virtual ~dMeshBVH();

		virtual void Build ();
		virtual void Cleanup ();
		
		void FaceRayCast (const dBigVector& l0, const dBigVector& l1, void* const userData) const;
		void GetOverlapNodes (dList<dgMeshBVHNode*>& overlapNodes, const dBigVector& p0, const dBigVector& p1) const;

		protected:
		virtual dgMeshBVHNode* CreateLeafNode (dEdge* const face, void* const userData) = 0;

		dgMeshBVHNode* AddFaceNode (dEdge* const face, void* const userData);
		void RemoveNode (dgMeshBVHNode* const treeNode);
		void ImproveNodeFitness ();
		void ImproveNodeFitness (dgMeshBVHNode* const node);
		dFloat32 CalculateSurfaceArea (dgMeshBVHNode* const node0, dgMeshBVHNode* const node1, dVector& minBox, dVector& maxBox) const;
		virtual bool SanityCheck() const;

		virtual dFloat64 RayFaceIntersect (const dgMeshBVHNode* const face, const dBigVector& p0, const dBigVector& p1, void* const userData) const;
//		virtual dFloat64 VertexRayCast (const dBigVector& l0, const dBigVector& l1) const;
//		virtual bool RayRayIntersect (dEdge* const edge, const dMeshEffect* const otherMesh, dEdge* const otherEdge, dFloat64& param, dFloat64& otherParam) const;
		
		const dMeshEffect* m_mesh;
		dgMeshBVHNode* m_rootNode;
		dgFitnessList m_fitness;
		friend class dMeshEffect;
	};

	dMeshEffect ();
	dMeshEffect(dMemoryAllocator___* const allocator);
	dMeshEffect(dgCollisionInstance* const collision);
	dMeshEffect(const dMeshEffect& source);
	dMeshEffect(dPolyhedra& mesh, const dMeshEffect& source);
	dMeshEffect (dMemoryAllocator___* const allocator, dgDeserialize deserialization, void* const userData);

	// create from OFF or PLY file format
	dMeshEffect(dMemoryAllocator___* const allocator, const char* const fileName);

	// Create a convex hull Mesh form point cloud
	dMeshEffect (dMemoryAllocator___* const allocator, const dFloat64* const vertexCloud, dInt32 count, dInt32 strideInByte, dFloat64 distTol);

	// create a planar Mesh
	dMeshEffect(dMemoryAllocator___* const allocator, const dMatrix& planeMatrix, dFloat32 witdth, dFloat32 breadth, dInt32 material, const dMatrix& textureMatrix0, const dMatrix& textureMatrix1);
	virtual ~dMeshEffect(void);

	void Init();

	void Trace () const;

	void ApplyTransform (const dMatrix& matrix);
	dMatrix CalculateOOBB (dBigVector& size) const;
	void CalculateAABB (dBigVector& min, dBigVector& max) const;

	void FlipWinding(); 
	void UniformBoxMapping (dInt32 material, const dMatrix& textureMatrix);
	void CalculateNormals (dFloat64 angleInRadians);
	void SphericalMapping (dInt32 material, const dMatrix& uvAligment);
	void BoxMapping (dInt32 front, dInt32 side, dInt32 top, const dMatrix& uvAligment);
	void CylindricalMapping (dInt32 cylinderMaterial, dInt32 capMaterial, const dMatrix& uvAligment);
	void AngleBaseFlatteningMapping (dInt32 cylinderMaterial, dgReportProgress progressReportCallback, void* const userData);

	dEdge* InsertEdgeVertex (dEdge* const edge, dFloat64 param);

	dMeshEffect* Union (const dMatrix& matrix, const dMeshEffect* const clipper) const;
	dMeshEffect* Difference (const dMatrix& matrix, const dMeshEffect* const clipper) const;
	dMeshEffect* Intersection (const dMatrix& matrix, const dMeshEffect* const clipper) const;
	void ClipMesh (const dMatrix& matrix, const dMeshEffect* const clipper, dMeshEffect** const top, dMeshEffect** const bottom) const;

	//bool PlaneClip (const dBigPlane& plane);
	
	dMeshEffect* ConvexMeshIntersection (const dMeshEffect* const convexMesh) const;

	dMeshEffect* GetFirstLayer ();
	dMeshEffect* GetNextLayer (dMeshEffect* const layer);

	void Triangulate ();
	void ConvertToPolygons ();
	void RemoveUnusedVertices(dInt32* const vertexRemapTable);
	
	void BeginBuild ();
		void BeginBuildFace ();
			void AddPoint (dFloat64 x, dFloat64 y, dFloat64 z);
			void AddLayer (dInt32 layer);
			void AddMaterial (dInt32 materialIndex);
			void AddNormal (dFloat32 x, dFloat32 y, dFloat32 z);
			void AddBinormal (dFloat32 x, dFloat32 y, dFloat32 z);
			void AddVertexColor (dFloat32 x, dFloat32 y, dFloat32 z, dFloat32 w);
			void AddUV0 (dFloat32 u, dFloat32 v);
			void AddUV1 (dFloat32 u, dFloat32 v);
		void EndBuildFace ();
	void EndBuild (dFloat64 tol, bool fixTjoint = true);

	dInt32 GetVertexCount() const;
	dInt32 GetVertexStrideInByte() const;
	const dFloat64* GetVertexPool () const;

	dInt32 GetVertexBaseCount() const;
	void SetVertexBaseCount(dInt32 count);
	
	dEdge* SpliteFace (dInt32 v0, dInt32 v1);

	dInt32 GetTotalFaceCount() const;
	dInt32 GetTotalIndexCount() const;
	void GetFaces (dInt32* const faceCount, dInt32* const materials, void** const faceNodeList) const;

	void RepairTJoints ();
	bool SeparateDuplicateLoops (dEdge* const face);

	bool HasOpenEdges () const;

	dFloat64 CalculateVolume () const;

	void OptimizePoints();
	void OptimizeAttibutes();
	void BuildFromIndexList(const dgMeshVertexFormat* const format);

	dInt32 GetPropertiesCount() const;
	const dInt32* GetIndexToVertexMap() const;

	bool HasLayersChannel() const;
	bool HasNormalChannel() const;
	bool HasBinormalChannel() const;
	bool HasUV0Channel() const;
	bool HasUV1Channel() const;
	bool HasVertexColorChannel() const;
	
	void GetVertexChannel64(dInt32 strideInByte, dFloat64* const bufferOut) const;
	void GetVertexChannel(dInt32 strideInByte, dFloat32* const bufferOut) const;
	void GetNormalChannel(dInt32 strideInByte, dFloat32* const bufferOut) const;
	void GetBinormalChannel(dInt32 strideInByte, dFloat32* const bufferOut) const;
	void GetUV0Channel(dInt32 strideInByte, dFloat32* const bufferOut) const;
	void GetUV1Channel(dInt32 strideInByte, dFloat32* const bufferOut) const;
	void GetVertexColorChannel(dInt32 strideInByte, dFloat32* const bufferOut) const;
//	void GetWeightBlendChannel(dInt32 strideInByte, dFloat32* const bufferOut) const;
//	void GetWeightIndexChannel(dInt32 strideInByte, dInt32* const bufferOut) const;

	dgIndexArray* MaterialGeometryBegin();
	void MaterialGeomteryEnd(dgIndexArray* const handle);
	dInt32 GetFirstMaterial (dgIndexArray* const handle) const;
	dInt32 GetNextMaterial (dgIndexArray* const handle, dInt32 materialHandle) const;
	dInt32 GetMaterialID (dgIndexArray* const handle, dInt32 materialHandle) const;
	dInt32 GetMaterialIndexCount (dgIndexArray* const handle, dInt32 materialHandle) const;
	void GetMaterialGetIndexStream (dgIndexArray* const handle, dInt32 materialHandle, dInt32* const index) const;
	void GetMaterialGetIndexStreamShort (dgIndexArray* const handle, dInt32 materialHandle, dgInt16* const index) const;
	
	dgCollisionInstance* CreateCollisionTree(dgWorld* const world, dInt32 shapeID) const;
	dgCollisionInstance* CreateConvexCollision(dgWorld* const world, dFloat64 tolerance, dInt32 shapeID, const dMatrix& matrix = dgGetIdentityMatrix()) const;

	dMeshEffect* CreateSimplification (dInt32 maxVertexCount, dgReportProgress reportProgressCallback, void* const userData) const;
	dMeshEffect* CreateConvexApproximation (dFloat32 maxConcavity, dFloat32 backFaceDistanceFactor, dInt32 maxHullOuputCount, dInt32 maxVertexPerHull, dgReportProgress reportProgressCallback, void* const userData) const;

	dMeshEffect* CreateTetrahedraIsoSurface() const;
	void CreateTetrahedraLinearBlendSkinWeightsChannel (const dMeshEffect* const tetrahedraMesh);

	static dMeshEffect* CreateVoronoiConvexDecomposition (dMemoryAllocator___* const allocator, dInt32 pointCount, dInt32 pointStrideInBytes, const dFloat32* const pointCloud, dInt32 materialId, const dMatrix& textureProjectionMatrix);
	static dMeshEffect* CreateFromSerialization (dMemoryAllocator___* const allocator, dgDeserialize deserialization, void* const userData);

	void LoadOffMesh (const char* const filename);
	void LoadTetraMesh (const char* const filename);
	void Serialize (dgSerialize callback, void* const userData) const;

	dBigVector GetVertex (dInt32 index) const;
	dInt32 GetVertexLayer (dInt32 index) const;

	void TransformMesh (const dMatrix& matrix);

	void* GetFirstVertex () const;
	void* GetNextVertex (const void* const vertex) const;
	int GetVertexIndex (const void* const vertex) const;

	void* GetFirstPoint () const;
	void* GetNextPoint (const void* const point) const;
	dInt32 GetPointIndex (const void* const point) const;
	dInt32 GetVertexIndexFromPoint (const void* const point) const;

	void* GetFirstEdge () const;
	void* GetNextEdge (const void* const edge) const;
//	void* FindEdge (dInt32 v0, dInt32 v1) const;
	void GetEdgeIndex (const void* const edge, dInt32& v0, dInt32& v1) const;
//	void GetEdgeAttributeIndex (const void* edge, dInt32& v0, dInt32& v1) const;

	const dEdge* GetPolyhedraEdgeFromNode(const void* const edge) const;

	void* GetFirstFace () const;
	void* GetNextFace (const void* const face) const;
	dInt32 IsFaceOpen (const void* const face) const;
	dInt32 GetFaceMaterial (const void* const face) const;
	dInt32 GetFaceIndexCount (const void* const face) const;
	void GetFaceIndex (const void* const face, dInt32* const indices) const;
	void GetFaceAttributeIndex (const void* const face, dInt32* const indices) const;
	dBigVector CalculateFaceNormal (const void* const face) const;

	void SetFaceMaterial (const void* const face, int materialID);
	void AddInterpolatedEdgeAttribute (dEdge* const edge, dFloat64 param);
	dInt32 AddInterpolatedHalfAttribute(dEdge* const edge, dInt32 midPoint);
	dInt32 InterpolateVertex (const dBigVector& point, const dEdge* const face) const;

	bool Sanity () const;

	protected:
	virtual void BeginFace();
	virtual bool EndFace ();

	dBigVector GetOrigin ()const;
	dInt32 CalculateMaxAttributes () const;
	dFloat64 QuantizeCordinade(dFloat64 val) const;

	void MergeFaces (const dMeshEffect* const source);
//	void ReverseMergeFaces (dMeshEffect* const source);

	bool PlaneClip (const dMeshEffect& convexMesh, const dEdge* const face);

	dMeshEffect* GetNextLayer (dInt32 mark);
	dMeshEffect* CreateVoronoiConvex (const dBigVector* const conevexPointCloud, dInt32 count, dInt32 materialId, const dMatrix& textureProjectionMatrix, dFloat32 normalAngleInRadians) const;

	void PackAttibuteData ();
	void UnpackAttibuteData ();

	void PackPoints (dFloat64 tol);
	void UnpackPoints();

	dgPointFormat m_points;
	dgAttibutFormat m_attrib;
	dInt32 m_vertexBaseCount;
	dInt32 m_constructionIndex;
	
	friend class dConvexHull3d;
	friend class dgConvexHull4d;
	friend class dgBooleanMeshBVH;
	friend class dgHACDClusterGraph;
	friend class dgTriangleAnglesToUV;
	friend class dgTetraIsoSufaceStuffing;
	friend class dgCollisionCompoundFractured;
#endif
};

#if 0
inline dInt32 dMeshEffect::GetVertexCount() const
{
	return m_points.m_vertex.m_count;
}

inline dInt32 dMeshEffect::GetVertexBaseCount() const
{
	return m_vertexBaseCount;
}

inline void dMeshEffect::SetVertexBaseCount(dInt32 count)
{
	m_vertexBaseCount = count;
}


inline dInt32 dMeshEffect::GetPropertiesCount() const
{
	return m_attrib.m_pointChannel.m_count;
}

inline const dInt32* dMeshEffect::GetIndexToVertexMap() const
{
	return &m_attrib.m_pointChannel[0];
}

inline dInt32 dMeshEffect::GetMaterialID (dgIndexArray* const handle, dInt32 materialHandle) const
{
	return handle->m_materials[materialHandle];
}

inline dInt32 dMeshEffect::GetMaterialIndexCount (dgIndexArray* const handle, dInt32 materialHandle) const
{
	return handle->m_materialsIndexCount[materialHandle];
}

inline dBigVector dMeshEffect::GetVertex (dInt32 index) const
{
	dAssert(index >= 0);
	dAssert(index < m_points.m_vertex.m_count);
	return m_points.m_vertex[index];
}

inline bool dMeshEffect::HasLayersChannel() const
{
	return m_points.m_layers.m_count != 0;
}

inline dInt32 dMeshEffect::GetVertexLayer(dInt32 index) const
{
	dAssert(index >= 0);
	dAssert(index < m_points.m_vertex.m_count);
	return (m_points.m_layers.m_count) ? m_points.m_layers[index] : 0;
}


inline dInt32 dMeshEffect::GetVertexStrideInByte() const 
{
	return sizeof (dBigVector);
}

inline const dFloat64* dMeshEffect::GetVertexPool () const 
{
	return &m_points.m_vertex[0].m_x;
}

inline dMeshEffect* dMeshEffect::GetFirstLayer ()
{
	return GetNextLayer (IncLRU());
}

inline dMeshEffect* dMeshEffect::GetNextLayer (dMeshEffect* const layerSegment)
{
	if (!layerSegment) {
		return nullptr;
	}
	return GetNextLayer (layerSegment->IncLRU() - 1);
}


inline dFloat64 dMeshEffect::QuantizeCordinade(dFloat64 x) const
{
	dInt32 exp;
	dFloat64 mantissa = frexp(x, &exp);
	mantissa = DG_MESH_EFFECT_PRECISION_SCALE_INV * floor (mantissa * DG_MESH_EFFECT_PRECISION_SCALE);

	dFloat64 x1 = ldexp(mantissa, exp);
	return x1;
}
#endif
#endif
