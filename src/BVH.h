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

	Hittable::Ptr left;	//	��Ϊ��������BVHNode��Ҳ���������壨Ҷ�ӽ�㣩
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
	//	�����᲻ͬ�Ƚ�����Ϊ��С��ĸ�������
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

	//	�ݹ��жϻ���
	bool hitLeft = left->hit(ray, tMin, tMax, rec);
	bool hitRight = right->hit(ray, tMin, tMax, rec);

	return hitLeft || hitRight;
}

BVHNode::BVHNode(const std::vector<Hittable::Ptr>& src_objects,
	size_t start, size_t end, double t0, double t1)
{
	auto objects = src_objects;

	//	���ѡȡһ������л���
	int axis = randomInt(0, 2);
	//	ѡ����lambda���ʽȡ��ԭ����boxCompareXYZ
	auto comparator = [=](const Hittable::Ptr a, const Hittable::Ptr b)->bool 
	{
		return boxCompare(a, b, axis); 
	};
	
	size_t objectsSpan = end - start;	//	BVH����Ӧ�����巶Χ

	//	�ݹ黮�֣�����BVH��
	//	������ֻʣһ�����壬������ָ���ָ�������
	if (objectsSpan == 1)
	{
		this->left = this->right = objects[start];
	}
	//	��������ʣ�������壬��ֱ��������Ҷ�ӽڵ�
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
	//	����ʣ�����������壬�������AABB�е�x,y,z����һ�����Сֵ����
	else
	{
		//	����������
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		//	�԰�ֹ�����������
		auto mid = start + objectsSpan / 2;
		this->left = make_shared<BVHNode>(objects, start, mid, t0, t1);
		this->right = make_shared<BVHNode>(objects, mid, end, t0, t1);
	}
	//	���ɵ�ǰBVH����boundingBox
	AABB leftBox, rightBox;
	if (!this->left->boundingBox(t0, t1, leftBox) || !this->right->boundingBox(t0, t1, rightBox))
	{
		//std::cerr << "No bounding box in bvh node constructor.\n";
	}
	mAABB = surroundingBox(leftBox, rightBox);
}

#endif // !_BVH_H
