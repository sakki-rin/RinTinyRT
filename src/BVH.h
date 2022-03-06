#pragma once
#ifndef _BVH_H
#define _BVH_H

#include "Hittable.h"
#include "HittableList.h"

class BVHNode : public Hittable
{
public:

	BVHNode() {}

	BVHNode(const HittableList& list, double t0,double t1)
		: BVHNode(list.objects,0,list.objects.size(),t0,t1) 
	{}

	BVHNode(const std::vector<Hittable::Ptr>& src_objects,
		size_t start, size_t end, double t0, double t1);

	virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRec) const override;

	virtual bool boundingBox(double t0, double t1, AABB& outputBox) const override;

public:

	Hittable::Ptr left;	//	因为结点可能是BVHNode，也可能是物体（叶子结点）
	Hittable::Ptr right;
	AABB mAABB;
};

inline bool boxCompare(const Hittable::Ptr a, const Hittable::Ptr b, int axis)
{
	AABB boxA;
	AABB boxB;

	if (!a->boundingBox(0, 0, boxA) || b->boundingBox(0, 0, boxB))
	{
		//std::cerr << "No bounding box in BVHNode constructor.\n";
	}
	//	根据轴不同比较依据为最小点的该轴坐标
	return boxA.min().e[axis] < boxB.min().e[axis];
}

bool boxCompareX(const Hittable::Ptr a, const Hittable::Ptr b)
{
	return boxCompare(a, b, 0);
}

bool boxCompareY(const Hittable::Ptr a, const Hittable::Ptr b)
{
	return boxCompare(a, b, 1);
}

bool boxCompareZ(const Hittable::Ptr a, const Hittable::Ptr b)
{
	return boxCompare(a, b, 2);
}

bool BVHNode::boundingBox(double t0, double t1, AABB& aabb) const
{
	aabb = mAABB;
	return true;
}

bool BVHNode::hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
	if (!mAABB.hit(ray, tMin, tMax))
	{
		return false;
	}

	//	递归判断击中
	bool hitLeft = left->hit(ray, tMin, tMax, rec);
	bool hitRight = right->hit(ray, tMin, tMax, rec);

	return hitLeft || hitRight;
}

BVHNode::BVHNode(const std::vector<Hittable::Ptr>& src_objects,
	size_t start, size_t end, double t0, double t1)
{
	auto objects = src_objects;

	//	随机选取一个轴进行划分
	int axis = randomInt(0, 2);
	//	选择用lambda表达式取代原来的boxCompareXYZ
	auto comparator = [=](const Hittable::Ptr a, const Hittable::Ptr b)->bool 
	{
		return boxCompare(a, b, axis); 
	};
	
	size_t objectsSpan = end - start;	//	BVH结点对应的物体范围

	//	递归划分，构建BVH树
	//	划分区只剩一个物体，则左右指针均指向该物体
	if (objectsSpan == 1)
	{
		this->left = this->right = objects[start];
	}
	//	划分区还剩两个物体，则分别加入左右叶子节点
	else if (objectsSpan == 2)
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			this->left = objects[start];
			this->right = objects[start + 1];
		}
		else
		{
			this->left = objects[start + 1];
			this->right = objects[start];
		}
	}
	//	若还剩三个以上物体，则按物体的AABB盒的x,y,z任意一轴的最小值排序
	else
	{
		//	对物体排序
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		//	对半分构造左右子树
		auto mid = start + objectsSpan / 2;
		this->left = make_shared<BVHNode>(objects, start, mid, t0, t1);
		this->right = make_shared<BVHNode>(objects, mid, end, t0, t1);
	}
	//	生成当前BVH结点的boundingBox
	AABB leftBox, rightBox;
	if (!this->left->boundingBox(t0, t1, leftBox) || !this->right->boundingBox(t0, t1, rightBox))
	{
		//std::cerr << "No bounding box in bvh node constructor.\n";
	}
	mAABB = surroundingBox(leftBox, rightBox);
}

#endif // !_BVH_H
