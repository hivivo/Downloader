//
//  AppDelegate.h
//  MacDownloader
//
//  Created by Xu Xu on 1/10/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;

@property (readonly, strong, nonatomic) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property (readonly, strong, nonatomic) NSManagedObjectModel *managedObjectModel;
@property (readonly, strong, nonatomic) NSManagedObjectContext *managedObjectContext;

- (IBAction)saveAction:(id)sender;


@property (weak) IBOutlet NSTextField *urlField;

- (IBAction)download:(id)sender;


@end
