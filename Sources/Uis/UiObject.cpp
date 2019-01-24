﻿#include "UiObject.hpp"

#include "Renderer/Renderer.hpp"
#include "Maths/Visual/DriverConstant.hpp"
#include "Uis.hpp"

namespace acid
{
	UiObject::UiObject(UiObject *parent, const UiBound &rectangle) :
		m_parent(parent),
		m_children(std::vector<std::unique_ptr<UiObject>>()),
		m_enabled(true),
		m_rectangle(UiBound(rectangle)),
		m_scissor(Vector4(0.0f, 0.0f, 1.0f, 1.0f)),
		m_depth(0.0f),
		m_screenDimension(Vector2()),
		m_screenPosition(Vector2()),
		m_lockRotation(true),
		m_worldTransform({}),
		m_alphaDriver(std::make_unique<DriverConstant>(1.0f)),
		m_alpha(1.0f),
		m_scaleDriver(std::make_unique<DriverConstant>(1.0f)),
		m_scale(1.0f),
		m_onClick(Delegate<void(UiObject *, MouseButton)>())
	{
		if (parent != nullptr)
		{
			parent->AddChild(this);
		}
	}

	UiObject::~UiObject()
	{
		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}
	}

	void UiObject::Update(std::vector<UiObject *> &list)
	{
		if (!m_enabled)
		{
			return;
		}

		if (Uis::Get()->GetSelector().IsSelected(*this))
		{
			for (uint32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
			{
				if (Uis::Get()->GetSelector().WasDown(static_cast<MouseButton>(i)))
				{
					m_onClick(this, static_cast<MouseButton>(i));
				}
			}
		}

		if (GetAlpha() > 0.0f)
		{
			UpdateObject();
			list.emplace_back(this);
		}

		// Alpha and scale updates.
		m_alpha = m_alphaDriver->Update(Engine::Get()->GetDelta());
		m_scale = m_scaleDriver->Update(Engine::Get()->GetDelta());

		// Transform updates.
		float aspectRatio = m_worldTransform ? 1.0f : Window::Get()->GetAspectRatio();

		Vector2 screenDimensions = m_rectangle.GetScreenDimensions(aspectRatio);
		Vector2 screenPosition = m_rectangle.GetScreenPosition(aspectRatio);

		m_screenDimension = screenDimensions * m_scale;
		m_screenPosition = screenPosition - (m_screenDimension * (Vector2::Down + m_rectangle.GetReference()));

		// Update all children objects.
		for (auto &child : m_children)
		{
			child->Update(list);
		}
	}

	void UiObject::UpdateObject()
	{
	}

	void UiObject::SetParent(UiObject *parent)
	{
		m_parent->RemoveChild(this);
		parent->AddChild(this);
		m_parent = parent;
	}

	void UiObject::AddChild(UiObject *child)
	{
		m_children.emplace_back(child);
	}

	void UiObject::RemoveChild(UiObject *child)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			if ((*it).get() == child)
			{
				m_children.erase(it);
			}
		}
	}

	bool UiObject::IsEnabled() const
	{
		if (m_parent != nullptr)
		{
			return m_enabled && m_parent->IsEnabled();
		}

		return m_enabled;
	}

	Matrix4 UiObject::GetModelMatrix() const
	{
		if (m_worldTransform)
		{
			Matrix4 worldMatrix = m_worldTransform->GetWorldMatrix();

			if (m_lockRotation)
			{
				Vector3 scaling = m_worldTransform->GetScaling();

				for (uint32_t i = 0; i < 3; i++)
				{
					worldMatrix[0][i] = scaling[i];
				}
			}

			return worldMatrix;
		}

		if (m_parent != nullptr)
		{
			return m_parent->GetModelMatrix(); // TODO: Multiply by this 'local' WorldMatrix.
		}

		return Matrix4::Identity;
	}

	float UiObject::GetAlpha() const
	{
		if (m_parent != nullptr)
		{
			return m_alpha * m_parent->GetAlpha();
		}

		return m_alpha;
	}

	void UiObject::CancelEvent(const MouseButton &button) const
	{
		Uis::Get()->GetSelector().CancelWasEvent(button);
	}
}
