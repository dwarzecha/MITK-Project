/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

#include <usModuleRegistry.h>

#include <QMessageBox>

#include <ExampleImageFilter.h>
#include <ExampleImageInteractor.h>
#include <mitkImagePixelReadAccessor.h>

#include "QmitkExampleView.h"
#include "KMeans.hpp"
#include <string>


const std::string QmitkExampleView::VIEW_ID = "K-Means Segmentation";

void QmitkExampleView::CreateQtPartControl(QWidget* parent)
{
	m_Controls.setupUi(parent);
	connect(m_Controls.processImageButton, SIGNAL(clicked()), this, SLOT(ProcessSelectedImage()));
}

void QmitkExampleView::SetFocus()
{
	m_Controls.processImageButton->setFocus();
}

void QmitkExampleView::OnSelectionChanged(berry::IWorkbenchPart::Pointer, const QList<mitk::DataNode::Pointer>& dataNodes)
{
	for (const auto& dataNode : dataNodes)
	{
		if (dataNode.IsNotNull() && nullptr != dynamic_cast<mitk::Image*>(dataNode->GetData()))
		{
			m_Controls.selectImageLabel->setVisible(false);
			return;
		}
	}

	m_Controls.selectImageLabel->setVisible(true);
}

void QmitkExampleView::ProcessSelectedImage()
{
	auto selectedDataNodes = this->GetDataManagerSelection();

	if (selectedDataNodes.empty())
		return;

	auto firstSelectedDataNode = selectedDataNodes.front();

	if (firstSelectedDataNode.IsNull())
	{
		QMessageBox::information(nullptr, "Example View", "Please load and select an image before starting image processing.");
		return;
	}

	auto data = firstSelectedDataNode->GetData();

	if (data != nullptr)
	{
		mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data);

		if (image.IsNotNull())
		{
			auto imageName = firstSelectedDataNode->GetName();
			auto offset = m_Controls.offsetSpinBox->value();

			MITK_INFO << "Process image \"" << imageName << "\" ...";

			std::vector<short> pixels;

			mitk::ImagePixelReadAccessor<short, 3> readAccess(image);

			for (unsigned int i = 0; i < image->GetDimension(0); i++)
			{
				for (unsigned int j = 0; j < image->GetDimension(1); j++)
				{
					for (unsigned int k = 0; k < image->GetDimension(2); k++)
					{
						itk::Index<3> idx = { { i, j, k } };
						short value = readAccess.GetPixelByIndex(idx);
						if (value != 0)
						{
							pixels.push_back(i);
							pixels.push_back(j);
							pixels.push_back(k);
							pixels.push_back(value);
						}
					}
				}
			}

			KMeans kmeans(pixels);
			kmeans.CalculateKMeans(1000, offset);
			kmeans.PrintToFile();

			MITK_INFO << "  done";
		}
	}
}
