// Copyright 2018 The SwiftShader Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef VK_DESCRIPTOR_POOL_HPP_
#define VK_DESCRIPTOR_POOL_HPP_

#include "VkObject.hpp"
#include <set>

namespace vk
{
	class DescriptorPool : public Object<DescriptorPool, VkDescriptorPool>
	{
	public:
		DescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, void* mem);
		~DescriptorPool() = delete;
		void destroy(const VkAllocationCallbacks* pAllocator);

		static size_t ComputeRequiredAllocationSize(const VkDescriptorPoolCreateInfo* pCreateInfo);

		VkResult allocateSets(uint32_t descriptorSetCount, const VkDescriptorSetLayout* pSetLayouts, VkDescriptorSet* pDescriptorSets);
		void freeSets(uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
		VkResult reset();

	private:
		VkResult allocateSets(size_t* sizes, uint32_t numAllocs, VkDescriptorSet* pDescriptorSets);
		VkDescriptorSet findAvailableMemory(size_t size);
		void freeSet(const VkDescriptorSet descriptorSet);
		size_t computeTotalFreeSize() const;

		struct Node
		{
			Node(VkDescriptorSet set, size_t size) : set(set), size(size) {}
			bool operator<(const Node& node) const { return this->set < node.set; }
			bool operator==(VkDescriptorSet set) const { return this->set == set; }

			VkDescriptorSet set;
			size_t size;
		};
		std::set<Node> nodes;

		VkDescriptorSet pool = nullptr;
		size_t poolSize = 0;
	};

	static inline DescriptorPool* Cast(VkDescriptorPool object)
	{
		return reinterpret_cast<DescriptorPool*>(object);
	}

} // namespace vk

#endif // VK_DESCRIPTOR_POOL_HPP_
