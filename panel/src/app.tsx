import {
  ApiFilled,
  CloudFilled,
  IeSquareFilled,
  ProfileFilled,
} from '@ant-design/icons';
import {makeStyles, Theme} from '@material-ui/core/styles';
import {Layout, Menu} from 'antd';
import 'antd/dist/antd.css';
import React from 'react';

const {Sider, Content} = Layout;

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    width: '100%',
  },
  siteLayout: {
    background: '#fff',
  },
  content: {
    background: '#fff',
  },
  sitePageHeaderGhostWrapper: {
    backgroundColor: '#f5f5f5',
  },
  mainLayout: {
    minHeight: '100%',
  },
}));

export function App(): JSX.Element {
  const classes = useStyles();

  return (
    <div className={classes.root}>
      <Layout className={classes.mainLayout}>
        <Sider collapsible>
          <Menu theme="dark" mode="inline" defaultSelectedKeys={['1']}>
            <Menu.Item key="1" icon={<ProfileFilled translate="no"/>}>
              Report Jobs
            </Menu.Item>
            <Menu.Item key="2" icon={<IeSquareFilled translate="no"/>}>
              Websites
            </Menu.Item>
            <Menu.Item key="3" icon={<ApiFilled translate="no"/>}>
              SKU Bindings
            </Menu.Item>
            <Menu.Item key="4" icon={<CloudFilled translate="no"/>}>
              Cloud Microservices
            </Menu.Item>
          </Menu>
        </Sider>
        <Layout className={classes.siteLayout}>
          <Content className={classes.content}>
            <div className={classes.sitePageHeaderGhostWrapper}>

            </div>
          </Content>
        </Layout>
      </Layout>
    </div>
  );
}
