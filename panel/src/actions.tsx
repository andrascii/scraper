import {PlusOutlined} from '@ant-design/icons';
import {makeStyles} from '@material-ui/core/styles';
import {Button, Divider, PageHeader} from 'antd';
import React from 'react';
import {openNotification} from './open_notification';
import {BrowserAction} from "./actions";

const useStyles = makeStyles(() => ({
  root: {
    width: '100%',
  },
}));

export function ReportJobs(): JSX.Element {
  const pollInterval = 5000;

  const websitesDefaultState: BrowserAction[] = [];

  const reportJobsDefaultState: GetReportJobsResponse = {
    records: [],
  };

  const classes = useStyles();
  const [websites, setWebsites] = React.useState(websitesDefaultState);
  const [reportJobs, setReportJobs] = React.useState(reportJobsDefaultState);
  const [showCreateReportJob, setShowCreateReportJob] = React.useState(false);

  const pollReportJobs = async (): Promise<void> => {
    try {
      setReportJobs(await ReportsModel.updateReportJobs());
    } catch (error) {
      openNotification('Fetching Report Jobs Error', `${error}`, 'error');
    }
  };

  React.useEffect(() => {
    ReportsModel.websites().then((websiteInfo: WebsiteInfo[]) => setWebsites(websiteInfo));
  }, []);

  React.useEffect(() => {
    const previousTitle = document.title;
    document.title = 'Report Jobs';
    return () => {
      document.title = previousTitle;
    };
    // tslint:disable-next-line: align
  }, []);

  React.useEffect(() => {
    const interval = setInterval(async () => pollReportJobs(), pollInterval);
    return () => clearInterval(interval);
    // tslint:disable-next-line: align
  }, []);

  const stillLoadingReportJobsInfo = !reportJobs.records.length;

  return (
    <PageHeader
      ghost={false}
      onBack={() => window.history.back()}
      title="Report Jobs"
      subTitle="watch/create/delete report jobs and download report results"
      extra={[
        <Button key="1" type="primary" onClick={() => setShowCreateReportJob(true)}>
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
          loading={stillLoadingReportJobsInfo}
          reportJobs={reportJobs}
          updateReportJobList={pollReportJobs}
        />
        <CreateReportJob
          visible={showCreateReportJob}
          setVisible={setShowCreateReportJob}
          websites={websites}
          onUpdate={pollReportJobs}
        />
      </div>
    </PageHeader>
  );
}
