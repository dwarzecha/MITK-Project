/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef QmitkExampleView_h
#define QmitkExampleView_h

#include <berryISelectionListener.h>
#include <QmitkAbstractView.h>

#include <ui_ExampleViewControls.h>

class QmitkExampleView : public QmitkAbstractView
{
	Q_OBJECT

public:
	 static const std::string VIEW_ID;
	 void CreateQtPartControl(QWidget* parent) override;

private slots:
	void ProcessSelectedImage();

private:
	void SetFocus() override;

	void OnSelectionChanged(
    berry::IWorkbenchPart::Pointer source,
    const QList<mitk::DataNode::Pointer>& dataNodes) override;

	Ui::ExampleViewControls m_Controls;
};

#endif
