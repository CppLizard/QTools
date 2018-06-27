#include "QTools.h"
#include "ui_QTools.h"

QTools::QTools(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTools)
{
    ui->setupUi(this);
    init();
}

QTools::~QTools()
{
    delete ui;
}

// ��ʼ��
void QTools::init()
{
    //���ش��ڵı�����
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | windowFlags());

    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);

    setWindowTitle(QString("�๦�ܹ�����"));
    setWindowIcon(QIcon(":/Icon/iconfont_48.png"));

//    QPalette pal(palette());
//    pal.setColor(QPalette::Background, QColor(0, 0, 0));
//    setAutoFillBackground(true);
//    setPalette(pal);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    /******************** ��ʱ�� ********************/
    m_pTimeTimer = new QTimer(this);
    connect(m_pTimeTimer,SIGNAL(timeout()),this,SLOT(on_timeTimer()));
    m_pTimeTimer->start(1000);

    /******************** ��ʼ��UI ********************/
    // ��ʼ��ToolBox
    initToolBox();
    // ��ȡʱ��
    ui->lbl_time->setText(Utils::getTime());
    // ��ȡIP��ַ
    ui->lbl_ip->setText(QString("IP:")+Utils::getHostIpAddress());
    // ����������ťͼ��
    Utils::setIcon(ui->btn_search, FontIcons::IconCode::icon_search, 14);

}

// ��ʼ��ToolBox
void QTools::initToolBox()
{
    ui->toolBox->removeItem(0);
    // �������
    m_softType[0] = "�ı�����";
    m_softType[1] = "ͼ����";
    m_softType[2] = "ѹ����ѹ";
    m_softType[3] = "���ܽ���";
    m_softType[4] = "���繤��";
    m_softType[5] = "ɨ�蹤��";
    m_softType[6] = "��ȫ����";
    m_softType[7] = "���ع���";
    m_softType[8] = "��������";
    m_softType[9] = "���Թ���";
    // �������
    m_softName[0][0] = "Notepad++";
    m_softName[0][1] = "SublimeText";
    // ���·��
    m_softPath[0][0] = "Notepad++.exe";
    m_softPath[0][1] = "SublimeText.exe";
    // ���ͼ��
    m_softIcon[0][0] = "iconfont_48.png";
    m_softIcon[0][1] = "iconfont_48.png";

    for (int i = 0; i < sizeof(m_softType)/sizeof(QString); i++)
    {
        // ������������ļ���
        QString dirPath = "./" + m_softType[i];
        QDir dir;
        // �ж��ļ����Ƿ����
        if (!dir.exists(dirPath))
        {
            // �����ڴ����ļ���
            dir.mkdir(dirPath);
        }
        // ����ListWidget
        m_pListWidget = new QListWidget();
        // �����źźͲ�
        connect(m_pListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_itemClicked(QListWidgetItem*)));
        // ʹ��QListView��ʾͼ��
        m_pListWidget->setViewMode(QListView::IconMode);
        // ���������Ƽ�����ͼ��
        for (int j = 0; j < sizeof(m_softName[0])/sizeof(m_softName[0][0]); j++)
        {
            if (m_softName[i][j].size() <= 0)
                break;
            m_pListWidgeItem = new QListWidgetItem();
            m_pListWidgeItem->setIcon(QIcon(":/Icon/"+m_softIcon[i][j]));
            m_pListWidgeItem->setText(m_softName[i][j]);
            m_pListWidget->addItem(m_pListWidgeItem);
        }
        // ����ͼ��Ĵ�С
        m_pListWidget->setIconSize(QSize(48, 48));
        // ��������Ĵ�С
        m_pListWidget->setGridSize(QSize(80, 80));
        // ����QListView��С�ı�ʱ��ͼ��ĵ���ģʽ���Զ�����
        m_pListWidget->setResizeMode(QListView::Adjust);
        // ����ͼ��ɲ������ƶ���Ĭ���ǿ��ƶ��ģ��ĳɾ�̬��
        m_pListWidget->setMovement(QListView::Static);
        // toolBox�����ListWidget
        ui->toolBox->addItem(m_pListWidget, m_softType[i]);
    }

}

// 1�붨ʱ����ˢ��ʱ��
void QTools::on_timeTimer()
{
    // ��ȡʱ��
    ui->lbl_time->setText(Utils::getTime());
}

// QListWidget����¼�
void QTools::on_itemClicked(QListWidgetItem* item)
{
    QString name = item->text();
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (m_softName[i][j].size() <= 0)
                break;
            if (m_softName[i][j] == name)
            {
                qDebug() << m_softName[i][j];
                qDebug() << m_softPath[i][j];
                return;
            }
        }
    }
}
