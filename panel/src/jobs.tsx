import {PlusOutlined} from '@ant-design/icons';
import {makeStyles} from '@material-ui/core/styles';
import {Button, Divider, PageHeader} from 'antd';
import React from 'react';
import {openNotification} from './open_notification';
import {DataModel} from './data_model'
import {Job} from './job'

const useStyles = makeStyles(() => ({
  root: {
    width: '100%',
  },
}));

export function Jobs(): JSX.Element {
  const pollInterval = 5000;

  const jobsDefaultState: Job[] = [];

  const classes = useStyles();
  const [jobs, setJobs] = React.useState(jobsDefaultState);
  const [showAddActionsPanel, setShowAddActionsPanel] = React.useState(false);

  const pollActions = async (): Promise<void> => {
    try {
      setJobs(await DataModel.fetchJobs());
    } catch (error) {
      openNotification('Fetching Jobs Error', `${error}`, 'error');
    }
  };

  React.useEffect(() => {
    const previousTitle = document.title;
    document.title = 'Jobs';
    return () => {
      document.title = previousTitle;
    };
    // tslint:disable-next-line: align
  }, []);

  React.useEffect(() => {
    const interval = setInterval(async () => pollActions(), pollInterval);
    return () => clearInterval(interval);
    // tslint:disable-next-line: align
  }, []);

  const stillLoading = !jobs.length;

  return (
    <PageHeader
      ghost={false}
      onBack={() => window.history.back()}
      title="Report Jobs"
      subTitle="watch/create/delete report jobs and download report results"
      extra={[
        <Button key="1" type="primary" onClick={() => setShowAddActionsPanel(true)}>
          <PlusOutlined translate="no"/>Create Report Job
        </Button>,
      ]}
    >
      <div
        className={classes.root}
      >
        <Divider>Statistics</Divider>
        {ReportJobsStatistics(reportJobs)}
        <ReportJobsTable
          loading={stillLoading}
          reportJobs={reportJobs}
          updateReportJobList={pollReportJobs}
        />
        <CreateReportJob
          visible={showAddActionsPanel}
          setVisible={setShowAddActionsPanel}
          websites={jobs}
          onUpdate={pollActions}
        />
      </div>
    </PageHeader>
  );
}
